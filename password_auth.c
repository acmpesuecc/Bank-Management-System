#include "password_auth.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERS_CSV "users.csv"

typedef struct {
    uint32_t state[8];
    uint32_t count[2];
    unsigned char buffer[64];
} SHA256_CTX;

static const uint32_t k[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

static uint32_t rotr(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }

static void sha256_init(SHA256_CTX *ctx) {
    ctx->count[0] = ctx->count[1] = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

static void sha256_transform(SHA256_CTX *ctx, const unsigned char *data) {
    uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];
    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | data[j + 3];
    for (; i < 64; ++i)
        m[i] = rotr(m[i - 2], 17) ^ rotr(m[i - 2], 19) ^ (m[i - 2] >> 10) + m[i - 7] +
               rotr(m[i - 15], 7) ^ rotr(m[i - 15], 18) ^ (m[i - 15] >> 3) + m[i - 16];
    a = ctx->state[0];
    b = ctx->state[1];
    c = ctx->state[2];
    d = ctx->state[3];
    e = ctx->state[4];
    f = ctx->state[5];
    g = ctx->state[6];
    h = ctx->state[7];
    for (i = 0; i < 64; ++i) {
        t1 = h + (rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25)) + ((e & f) ^ (~e & g)) + k[i] + m[i];
        t2 = (rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }
    ctx->state[0] += a;
    ctx->state[1] += b;
    ctx->state[2] += c;
    ctx->state[3] += d;
    ctx->state[4] += e;
    ctx->state[5] += f;
    ctx->state[6] += g;
    ctx->state[7] += h;
}

static void sha256_update(SHA256_CTX *ctx, const unsigned char *data, size_t len) {
    size_t i, index = ctx->count[0] / 8 % 64;
    if ((ctx->count[0] += len << 3) < (len << 3)) ctx->count[1]++;
    ctx->count[1] += len >> 29;
    for (i = 0; i < len; ++i) {
        ctx->buffer[index++] = data[i];
        if (index == 64) {
            sha256_transform(ctx, ctx->buffer);
            index = 0;
        }
    }
}

static void sha256_final(SHA256_CTX *ctx, unsigned char *hash) {
    unsigned char finalcount[8];
    size_t i, index = ctx->count[0] / 8 % 64;
    for (i = 0; i < 8; i++)
        finalcount[i] = (unsigned char)((ctx->count[i >= 4 ? 0 : 1] >> ((3 - (i & 3)) * 8)) & 255);
    sha256_update(ctx, (unsigned char *)"\200", 1);
    while (ctx->count[0] / 8 % 64 != 56) sha256_update(ctx, (unsigned char *)"", 1);
    sha256_update(ctx, finalcount, 8);
    for (i = 0; i < 32; i++)
        hash[i] = (unsigned char)((ctx->state[i >> 2] >> ((3 - (i & 3)) * 8)) & 255);
}

static void sha256_hex(const char *input, char *out_hex) {
    SHA256_CTX ctx;
    unsigned char hash[32];
    int i;
    sha256_init(&ctx);
    sha256_update(&ctx, (const unsigned char *)input, strlen(input));
    sha256_final(&ctx, hash);
    for (i = 0; i < 32; i++) sprintf(out_hex + (i * 2), "%02x", hash[i]);
    out_hex[64] = '\0';
}

int auth_hash_password(const char *password, const char *salt, char *out_hex) {
    char payload[256];
    if (!password || !salt || !out_hex) return 0;
    snprintf(payload, sizeof(payload), "%s%s", salt, password);
    sha256_hex(payload, out_hex);
    return 1;
}

int auth_generate_salt(char *out_salt) {
    static const char hex[] = "0123456789abcdef";
    int i;
    unsigned seed = (unsigned)time(NULL) ^ (unsigned)rand();
    if (!out_salt) return 0;
    for (i = 0; i < 32; i++) {
        seed = seed * 1103515245 + 12345;
        out_salt[i] = hex[(seed >> 16) & 15];
    }
    out_salt[32] = '\0';
    return 1;
}

static int write_users_csv_line(const char *account_id, const char *salt, const char *hash_hex) {
    FILE *fp = fopen(USERS_CSV, "a");
    if (!fp) return 0;
    fprintf(fp, "%s,%s,%s\n", account_id, salt, hash_hex);
    fclose(fp);
    return 1;
}

static int read_credential(const char *account_id, char *salt_out, char *hash_out) {
    FILE *fp = fopen(USERS_CSV, "r");
    char line[256], id[64], salt[PASSWORD_SALT_LEN], hash[PASSWORD_HASH_HEX_LEN];
    if (!fp) return 0;
    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "%63[^,],%32s,%64s", id, salt, hash) == 3 && strcmp(id, account_id) == 0) {
            strcpy(salt_out, salt);
            strcpy(hash_out, hash);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

int auth_register_credential(const char *account_id, const char *password) {
    char salt[PASSWORD_SALT_LEN];
    char hash_hex[PASSWORD_HASH_HEX_LEN];
    if (!account_id || !password || strlen(password) == 0) return 0;
    if (!auth_generate_salt(salt)) return 0;
    if (!auth_hash_password(password, salt, hash_hex)) return 0;
    return write_users_csv_line(account_id, salt, hash_hex);
}

int auth_verify_credential(const char *account_id, const char *password) {
    char salt[PASSWORD_SALT_LEN];
    char stored_hash[PASSWORD_HASH_HEX_LEN];
    char computed_hash[PASSWORD_HASH_HEX_LEN];
    if (!account_id || !password) return 0;
    if (!read_credential(account_id, salt, stored_hash)) return 0;
    if (!auth_hash_password(password, salt, computed_hash)) return 0;
    return strcmp(stored_hash, computed_hash) == 0;
}

int auth_change_password(const char *account_id, const char *new_password) {
    FILE *in, *out;
    char line[256], id[64], salt[PASSWORD_SALT_LEN], hash[PASSWORD_HASH_HEX_LEN];
    char new_salt[PASSWORD_SALT_LEN];
    char new_hash[PASSWORD_HASH_HEX_LEN];
    int updated = 0;
    if (!account_id || !new_password) return 0;
    if (!auth_generate_salt(new_salt)) return 0;
    if (!auth_hash_password(new_password, new_salt, new_hash)) return 0;
    in = fopen(USERS_CSV, "r");
    out = fopen("users.csv.tmp", "w");
    if (!in || !out) {
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }
    while (fgets(line, sizeof(line), in)) {
        if (sscanf(line, "%63[^,],%32s,%64s", id, salt, hash) == 3 && strcmp(id, account_id) == 0) {
            fprintf(out, "%s,%s,%s\n", account_id, new_salt, new_hash);
            updated = 1;
        } else {
            fputs(line, out);
        }
    }
    fclose(in);
    fclose(out);
    if (!updated) {
        remove("users.csv.tmp");
        return 0;
    }
    remove(USERS_CSV);
    rename("users.csv.tmp", USERS_CSV);
    return 1;
}

int auth_ensure_default_admin(void) {
    FILE *fp = fopen(USERS_CSV, "r");
    if (fp) {
        fclose(fp);
        return 1;
    }
    return auth_register_credential("__admin__", "admin123");
}

#ifndef PASSWORD_AUTH_H
#define PASSWORD_AUTH_H

#define PASSWORD_HASH_HEX_LEN 65
#define PASSWORD_SALT_LEN 33

int auth_hash_password(const char *password, const char *salt, char *out_hex);
int auth_generate_salt(char *out_salt);
int auth_register_credential(const char *account_id, const char *password);
int auth_verify_credential(const char *account_id, const char *password);
int auth_change_password(const char *account_id, const char *new_password);
int auth_ensure_default_admin(void);

#endif

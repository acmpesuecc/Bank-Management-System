"""Unit tests for password hashing used by password_auth.c (SHA-256(salt+password))."""

import hashlib
import unittest


def hash_password(password: str, salt: str) -> str:
    payload = f"{salt}{password}"
    return hashlib.sha256(payload.encode("utf-8")).hexdigest()


class PasswordAuthTests(unittest.TestCase):
    def test_hash_is_deterministic(self):
        salt = "abc123def456"
        h1 = hash_password("secret", salt)
        h2 = hash_password("secret", salt)
        self.assertEqual(h1, h2)
        self.assertEqual(len(h1), 64)

    def test_different_passwords_differ(self):
        salt = "fixedsalt000000000000000000000000"
        self.assertNotEqual(
            hash_password("password1", salt),
            hash_password("password2", salt),
        )

    def test_different_salts_differ(self):
        password = "samepassword"
        self.assertNotEqual(
            hash_password(password, "salt1"),
            hash_password(password, "salt2"),
        )

    def test_no_plaintext_in_stored_record(self):
        password = "userpass123"
        salt = "0123456789abcdef0123456789abcdef"
        stored = f"ACC001,{salt},{hash_password(password, salt)}"
        self.assertNotIn(password, stored)
        self.assertIn(salt, stored)


if __name__ == "__main__":
    unittest.main()

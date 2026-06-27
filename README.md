# Banking Management System

This is a console-based Banking Management System written in C. The application allows users to perform banking operations such as creating a new account, viewing existing accounts, updating customer details, managing transactions, and deleting accounts.  
- Maintainer: [Sarah Kazi](https://github.com/Sarah-Kazi)

## Features

- **Create New Account**: Users can create a new bank account by providing details like name, date of birth, address, citizenship number, phone number, account type, and initial deposit.
- **View Account List**: Lists all the existing accounts with account numbers, customer names, addresses, and phone numbers.
- **Edit Account Information**: Allows editing of customer details such as address and phone number.
- **Transactions**: Users can deposit or withdraw money from an account.
- **Delete Account**: Removes an account from the database.
- **View Account Details**: Displays detailed information about a specific account, including interest calculation for fixed and saving accounts.
- **Password Authentication**: Account passwords are hashed with SHA-256 + per-user salt and stored in `users.csv` (no plaintext). Login required for customer operations.

## Authentication

- New accounts set a password at creation (stored as `account_id,salt,sha256_hex` in `users.csv`).
- Default admin credential is created on first run (`__admin__` / `admin123` until changed).
- Run unit tests: `python -m unittest tests/test_password_auth.py -v`

## How to Run

1. Clone this repository.
2. Compile the code using any C compiler. For example, with GCC:  
   gcc -o bank_management bank_management_system.c password_auth.c
3. Run the executable:  
   bank_management.exe

## Preview

### Main Menu
![Main Menu](./assets/main_menu.png)

### Transaction Receipt
![Transaction Receipt](./assets/receipt.png)

---

📂 **Note**: The `assets/` folder contains the screenshots for the **Main Menu** and **Transaction Receipt** shown above.


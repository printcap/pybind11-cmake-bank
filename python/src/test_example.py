"""
Python binding tests of example library
"""
from example import gcd, Account, Bank


def test_gcd_same_number():
  assert gcd(42, 42) == 42

def test_gcd_prime():
  assert gcd(11, 23) == 1

def test_gcd_non_prime():
  assert gcd(56, 42) == 14

def test_gcd_first_arg_zero():
  assert gcd(0, 42) == 42

def test_gcd_second_arg_zero():
  assert gcd(42, 0) == 42

def test_account_new_account_zero_balance_correct_name():
  account = Account(42, 'Dagobert')
  assert account.get_balance() == 0
  assert account.get_owner_name() == 'Dagobert'
  assert account.get_id() == 42

def test_account_deposit_pos_amount():
  account = Account(42, 'Dagobert')
  account.deposit(100)
  assert account.get_balance() == 100

def test_account_deposit_neg_amount_fails():
  account = Account(42, 'Dagobert')
  account.deposit(-100)
  assert account.get_balance() == 0

def test_account_withdrawal_pos_sufficient_amount():
  account = Account(42, 'Dagobert')
  account.deposit(100)
  account.withdraw(30)
  assert account.get_balance() == 70

def test_account_withdrawal_insufficient_amount_fails():
  account = Account(42, 'Dagobert')
  account.deposit(10)
  account.withdraw(50)
  assert account.get_balance() == 10

def test_account_withdrawal_neg_amount_fails():
  account = Account(42, 'Dagobert')
  account.deposit(100)
  account.withdraw(-50)
  assert account.get_balance() == 100


def test_new_account_can_be_found_and_is_correct():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  account = bank.find_account(account_id)
  assert account.get_id() == account_id
  assert account.get_owner_name() == 'Dagobert'
  assert account.get_balance() == 0

def test_account_for_invalid_id_cannot_be_found():
  bank = Bank()
  account = bank.find_account(42)
  assert account == 'account not found'

def test_deposit_in_bank_account():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  bank.deposit(account_id, 100)
  account = bank.find_account(account_id)
  assert account.get_balance() == 100

def test_deposit_with_neg_amount_in_bank_account_fails():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  err = bank.deposit(account_id, -100)
  assert err == 'negative amount'
  account = bank.find_account(account_id)
  assert account.get_balance() == 0

def test_deposit_in_nonexistent_account_in_bank_fails():
  bank = Bank()
  invalid_id = 42
  err = bank.deposit(invalid_id, 100)
  assert err == 'account not found'

def test_withdrawal_with_sufficient_funds_in_bank():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  bank.deposit(account_id, 100)
  bank.withdraw(account_id, 30)
  account = bank.find_account(account_id)
  assert account.get_balance() == 70

def test_withdrawal_with_insufficient_funds_in_bank_fails():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  bank.deposit(account_id, 100)
  err = bank.withdraw(account_id, 200)
  assert err == 'insufficient funds'

def test_withdrawal_neg_amount_in_bank_fails():
  bank = Bank()
  account_id = bank.create_account('Dagobert')
  bank.deposit(account_id, 100)
  err = bank.withdraw(account_id, -100)
  assert err == 'negative amount'

def test_withdrawal_from_nonexistent_account_in_bank_fails():
  bank = Bank()
  invalid_id = 42
  err = bank.withdraw(invalid_id, 100)
  assert err == 'account not found'

def test_transfer_pos_amount():
  bank = Bank()
  from_id = bank.create_account('Dagobert')
  to_id = bank.create_account('Donald')
  bank.deposit(from_id, 100)
  bank.transfer(from_id, to_id, 30)
  bank.find_account(from_id).get_balance() == 70
  bank.find_account(to_id).get_balance() == 30

def test_transfer_for_insufficient_funds_fails():
  bank = Bank()
  from_id = bank.create_account('Dagobert')
  to_id = bank.create_account('Donald')
  bank.deposit(from_id, 100)
  err = bank.transfer(from_id, to_id, 200)
  assert err == 'insufficient funds'
  bank.find_account(from_id).get_balance() == 100
  bank.find_account(to_id).get_balance() == 0

def test_transfer_for_neg_amount_fails():
  bank = Bank()
  from_id = bank.create_account('Dagobert')
  to_id = bank.create_account('Donald')
  bank.deposit(from_id, 100)
  err = bank.transfer(from_id, to_id, -10)
  assert err == 'negative amount'
  bank.find_account(from_id).get_balance() == 100
  bank.find_account(to_id).get_balance() == 0

def test_transfer_from_nonexistent_account_fails():
  bank = Bank()
  from_id = 42
  to_id = bank.create_account('Donald')
  err = bank.transfer(from_id, to_id, 10)
  assert err == 'source account not found'
  bank.find_account(to_id).get_balance() == 0

def test_transfer_to_nonexistent_account_fails():
  bank = Bank()
  from_id = bank.create_account('Dagobert')
  bank.deposit(from_id, 100)
  to_id = 42
  err = bank.transfer(from_id, to_id, 10)
  assert err == 'destination account not found'
  bank.find_account(from_id).get_balance() == 100

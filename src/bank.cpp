#include <example/example.hpp>

namespace example {

void Account::deposit(Amount amount) {
  if (amount > 0) {
    balance_ += amount;
  }
}

void Account::withdraw(Amount amount) {
  if ((amount > 0) && (balance_ >= amount)) {
    balance_ -= amount;
  }
}

AccountID Bank::create_account(const std::string& owner_name) {
  auto id = Bank::next_account_id++;
  accounts_.emplace(std::make_pair(id, Account(id, owner_name)));
  return id;
}

static std::string error_account_not_found = "account not found";
static std::string error_source_account_not_found = "source account not found";
static std::string error_destination_account_not_found= "destination account not found";
static std::string error_negative_amount = "negative amount";
static std::string error_insufficent_funds = "insufficient funds";


AccountOrError Bank::find_account(AccountID account_id) {
  auto it = accounts_.find(account_id);
  if (it == accounts_.end()) {
    return error_account_not_found;
  }
  return it->second;
}

MaybeError Bank::deposit(AccountID account_id, Amount amount) {
  auto it = accounts_.find(account_id);
  if (it == accounts_.end()) {
    return error_account_not_found;
  }
  if (amount < 0) {
    return error_negative_amount;
  }
  auto& account = it->second;
  account.deposit(amount);
  return MaybeError{};
}

MaybeError Bank::withdraw(AccountID account_id, Amount amount) {
  auto it = accounts_.find(account_id);
  if (it == accounts_.end()) {
    return error_account_not_found;
  }
  if (amount < 0) {
    return error_negative_amount;
  }
  auto& account = it->second;
  if (amount > account.get_balance()) {
    return error_insufficent_funds;
  }
  account.withdraw(amount);
  return MaybeError{};
}

MaybeError Bank::transfer(AccountID from_account, AccountID to_account, Amount amount) {
  auto from_it = accounts_.find(from_account);
  if (from_it == accounts_.end()) {
    return error_source_account_not_found;
  }
  auto& src_account = from_it->second;
  auto to_it = accounts_.find(to_account);
  if (to_it == accounts_.end()) {
    return error_destination_account_not_found;
  }
  auto& dest_account = to_it->second;
  if (amount < 0) {
    return error_negative_amount;
  }
  if (amount > src_account.get_balance()) {
    return error_insufficent_funds;
  }
  src_account.withdraw(amount);
  dest_account.deposit(amount);
  return MaybeError{};
}


} // end of namespace example

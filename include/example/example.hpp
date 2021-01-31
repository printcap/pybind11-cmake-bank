#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <atomic>
#include <cinttypes>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

namespace example {

///! Computes greatest common divisor of two numbers
uint64_t gcd(uint64_t a, uint64_t b);

using AccountID = uint64_t;
using Amount = int64_t;

class Account {
 public:
  explicit Account(AccountID account_id, std::string owner_name)
   : account_id_(account_id), owner_name_(owner_name) { }

  void deposit(Amount amount);
  void withdraw(Amount amount);

  AccountID get_id() const { return account_id_; }
  std::string get_owner_name() const { return owner_name_; }
  void set_owner_name(std::string owner_name) { owner_name_ = owner_name; }
  Amount get_balance () const { return balance_; }

 private:
  const AccountID account_id_;
  std::string owner_name_;
  Amount balance_ = 0L;
};

using AccountOrError = std::variant<Account, std::string>;
using MaybeError = std::optional<std::string>;

class Bank {
 public:
  Bank() { }
  ~Bank() { }
  AccountID create_account(const std::string& owner_name);
  AccountOrError find_account(AccountID account_id);
  MaybeError deposit(AccountID account_id, Amount amount);
  MaybeError withdraw(AccountID account_id, Amount amount);
  MaybeError transfer(AccountID from_account, AccountID to_account, Amount amount);


 private:
  std::unordered_map<AccountID, Account> accounts_;
  std::atomic<AccountID> next_account_id{1L};
};

} // end of namespace example

#endif // EXAMPLE_HPP

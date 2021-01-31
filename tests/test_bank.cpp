#include "catch.hpp"

#include <example/example.hpp>

using example::Account;
using example::Bank;

TEST_CASE("new account has zero balance and correct name", "[account]" ) {
  Account account{42UL, "Dagobert Duck"};
  REQUIRE(account.get_balance() == 0);
  REQUIRE(account.get_id() == 42UL);
  REQUIRE(account.get_owner_name() == "Dagobert Duck");
}

TEST_CASE("deposit of pos amount works", "[account]") {
  Account account{42UL, "Dagobert Duck"};
  account.deposit(100);
  REQUIRE(account.get_balance() == 100);
}

TEST_CASE("deposit of neg does not work", "[account]") {
  Account account{42UL, "Dagobert Duck"};
  account.deposit(100);
  account.deposit(-10);
  REQUIRE(account.get_balance() == 100);
}

TEST_CASE("withdrawal of pos amount works of sufficient funds available", "[account]") {
  Account account{42UL, "Dagobert Duck"};
  account.deposit(100);
  account.withdraw(10);
  REQUIRE(account.get_balance() == 90);
}


TEST_CASE("newly created account can be found and is correct", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  auto maybeAccount = bank.find_account(id);
  REQUIRE(maybeAccount.index() == 0);
  auto account = std::get<Account>(maybeAccount);
  REQUIRE(account.get_balance() == 0);
  REQUIRE(account.get_id() == 1UL);
  REQUIRE(account.get_owner_name() == "Dagobert");
}

TEST_CASE("account with invalid id cannot be found", "[bank]" ) {
  Bank bank;
  auto maybeAccount = bank.find_account(42);
  REQUIRE(maybeAccount.index() == 1);
  REQUIRE(std::get<std::string>(maybeAccount) == "account not found");
}

TEST_CASE("deposit of pos amount on existing account in bank works", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  auto maybeError = bank.deposit(id, 1000);
  REQUIRE_FALSE(maybeError.has_value());
   REQUIRE(std::get<Account>(bank.find_account(id)).get_balance() == 1000);
}

TEST_CASE("deposit of neg amount on existing account in bank fails", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  auto maybeError = bank.deposit(id, -1000);
  REQUIRE((maybeError.has_value() && *maybeError == "negative amount"));
}

TEST_CASE("deposit on non-existing account in bank fails", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  auto maybeError = bank.deposit(id + 1, 1000);
  REQUIRE((maybeError.has_value() && *maybeError == "account not found"));
}

TEST_CASE("withdrawal of pos amount on existing account with sufficient balance in bank works", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  bank.deposit(id, 1000);
  auto maybeError = bank.withdraw(id, 100);
  REQUIRE_FALSE(maybeError.has_value());
  REQUIRE(std::get<Account>(bank.find_account(id)).get_balance() == 900);
}

TEST_CASE("withdrawal on existing account with insufficient funds in bank fails", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  bank.deposit(id, 100);
  auto maybeError = bank.withdraw(id, 200);
  REQUIRE((maybeError.has_value() && *maybeError == "insufficient funds"));
}

TEST_CASE("withdrawal of neg amount on existing account in bank fails", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  bank.deposit(id, 1000);
  auto maybeError = bank.withdraw(id, -1000);
  REQUIRE((maybeError.has_value() && *maybeError == "negative amount"));
}

TEST_CASE("withdrawal on non-existing account in bank fails", "[bank]" ) {
  Bank bank;
  auto id = bank.create_account("Dagobert");
  auto maybeError = bank.deposit(id + 1, 1000);
  REQUIRE((maybeError.has_value() && *maybeError == "account not found"));
}

TEST_CASE("transfer of pos amount between two accounts successful", "[bank]") {
  Bank bank;
  auto from_id = bank.create_account("Dagobert");
  auto to_id = bank.create_account("Donald");
  bank.deposit(from_id, 100);
  auto maybeError = bank.transfer(from_id, to_id, 30);
  REQUIRE_FALSE(maybeError);
  REQUIRE(std::get<Account>(bank.find_account(from_id)).get_balance() == 70);
  REQUIRE(std::get<Account>(bank.find_account(to_id)).get_balance() == 30);
}

TEST_CASE("transfer of two accounts fails for insufficient funds", "[bank]") {
  Bank bank;
  auto from_id = bank.create_account("Dagobert");
  auto to_id = bank.create_account("Donald");
  bank.deposit(from_id, 100);
  auto maybeError = bank.transfer(from_id, to_id, 200);
  REQUIRE((maybeError && *maybeError == "insufficient funds"));
  REQUIRE(std::get<Account>(bank.find_account(from_id)).get_balance() == 100);
  REQUIRE(std::get<Account>(bank.find_account(to_id)).get_balance() == 0);
}

TEST_CASE("transfer of two accounts fails for neg amount", "[bank]") {
  Bank bank;
  auto from_id = bank.create_account("Dagobert");
  auto to_id = bank.create_account("Donald");
  bank.deposit(from_id, 100);
  auto maybeError = bank.transfer(from_id, to_id, -10);
  REQUIRE((maybeError && *maybeError == "negative amount"));
  REQUIRE(std::get<Account>(bank.find_account(from_id)).get_balance() == 100);
  REQUIRE(std::get<Account>(bank.find_account(to_id)).get_balance() == 0);
}

TEST_CASE("transfer of two accounts fails if source account does not exist", "[bank]") {
  Bank bank;
  auto from_id = bank.create_account("Dagobert");
  auto to_id = bank.create_account("Donald");
  bank.deposit(from_id, 100);
  auto maybeError = bank.transfer(from_id + 10, to_id, 10);
  REQUIRE((maybeError && *maybeError == "source account not found"));
  REQUIRE(std::get<Account>(bank.find_account(from_id)).get_balance() == 100);
  REQUIRE(std::get<Account>(bank.find_account(to_id)).get_balance() == 0);
}

TEST_CASE("transfer of two accounts fails if destination account does not exist", "[bank]") {
  Bank bank;
  auto from_id = bank.create_account("Dagobert");
  auto to_id = bank.create_account("Donald");
  bank.deposit(from_id, 100);
  auto maybeError = bank.transfer(from_id, to_id + 10, 10);
  REQUIRE((maybeError && *maybeError == "destination account not found"));
  REQUIRE(std::get<Account>(bank.find_account(from_id)).get_balance() == 100);
  REQUIRE(std::get<Account>(bank.find_account(to_id)).get_balance() == 0);
}
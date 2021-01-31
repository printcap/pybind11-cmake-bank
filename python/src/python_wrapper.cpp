#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <example/example.hpp>

#include <sstream>

namespace py = pybind11;

using namespace example;

PYBIND11_MODULE(example, m) {
  m.doc() = "pybind11 example plugin";  // optional module docstring
  m.def("gcd", &gcd, "Greatest common divisor of two positive integers");
  py::class_<Account>(m, "Account")
    .def(py::init<AccountID,  std::string>())
    .def("deposit", &Account::deposit)
    .def("withdraw", &Account::withdraw)
    .def("get_id", &Account::get_id)
    .def("get_balance", &Account::get_balance)
    .def("get_owner_name", &Account::get_owner_name)
    .def("set_owner_name", &Account::set_owner_name)
    .def("__repr__",
         [](const Account& a) {
           std::ostringstream oss;
           oss << "<Account id:" <<a.get_id() << " owner_name:'" << a.get_owner_name()
            << "' balance:" << a.get_balance() << '>';
           return oss.str();
         }
    );
  py::class_<Bank>(m, "Bank")
    .def(py::init())
    .def("create_account", &Bank::create_account)
    .def("find_account", &Bank::find_account)
    .def("deposit", &Bank::deposit)
    .def("withdraw", &Bank::withdraw)
    .def("transfer", &Bank::transfer)
  ;
}

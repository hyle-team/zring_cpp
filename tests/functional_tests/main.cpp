// Copyright (c) 2014-2018 Zano Project
// Copyright (c) 2014-2018 The Louisdor Project
// Copyright (c) 2012-2013 The Cryptonote developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "helpers.h"
#include <boost/program_options.hpp>
#include "common/command_line.h"
#include "crypto_tests.h"

namespace po = boost::program_options;

namespace
{
  const command_line::arg_descriptor<bool> arg_help = {"help", "show help"};
  const command_line::arg_descriptor<std::string> arg_crypto_tests = { "crypto-tests", "Run experimental crypto tests", "", true };
}


int main(int argc, char* argv[])
{
  try
  {
    po::options_description desc_options("Allowed options");
    command_line::add_arg(desc_options, arg_help);
    command_line::add_arg(desc_options, arg_crypto_tests);

    po::variables_map vm;
    bool r = command_line::handle_error_helper(desc_options, [&]()
    {
      po::store(po::parse_command_line(argc, argv, desc_options), vm);
      po::notify(vm);
      return true;
    });
    if (!r)
      return 1;

    if (command_line::get_arg(vm, arg_help))
    {
      std::cout << desc_options << std::endl;
      return 0;
    }

    if (command_line::has_arg(vm, arg_crypto_tests))
    {
      return crypto_tests(command_line::get_arg(vm, arg_crypto_tests));
    }
    else
    {
      std::cout << desc_options << std::endl;
      return 1;
    }
  }
  catch (std::exception& e)
  {
    LOG_ERROR("EXCEPTION: " << e.what());
  }

  return 0;
}

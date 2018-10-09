/*
 *
 * Copyright 2018 Asylo authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>

#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"

#include "asylo/grpc/auth/enclave_server_credentials.h"
#include "asylo/grpc/auth/null_credentials_options.h"
#include "asylo/grpc/util/enclave_server.h"
#include "asylo/trusted_application.h"
#include "asylo/util/status.h"

#include "oak/oak.pb.h"
#include "oak/oak_server.h"

#include "lisp.h"
#include "wac/wa.h"

class OakApplication : public asylo::TrustedApplication {
public:
  OakApplication() {}

  asylo::Status Run(const asylo::EnclaveInput &input,
                    asylo::EnclaveOutput *output) override {
    if (input.HasExtension(oak::initialise_input)) {
      oak::InitialiseInput in = input.GetExtension(oak::initialise_input);
      oak::InitialiseOutput out = this->Initialise(in);
      *output->MutableExtension(oak::initialise_output) = out;
    } else if (input.HasExtension(oak::evaluate_input)) {
      oak::EvaluateInput in = input.GetExtension(oak::evaluate_input);
      oak::EvaluateOutput out = this->Evaluate(in);
      *output->MutableExtension(oak::evaluate_output) = out;
    } else {
      return asylo::Status(asylo::error::GoogleError::INVALID_ARGUMENT,
                           "Expected extension on input.");
    }
    return asylo::Status::OkStatus();
  }

private:
  environment env;
  Module *module;

  oak::InitialiseOutput Initialise(oak::InitialiseInput input) {
    add_globals(this->env);
    add_oak_intrinsics(this->env);
    std::stringstream ss(input.wasm_module());

    Options opts {
      .disable_memory_bounds = false,
      .mangle_table_index = false,
      .dlsym_trim_underscore = false,
    };
    LOG(INFO) << opts.disable_memory_bounds;

    //load_module((uint8_t *) input.wasm_module().c_str(), opts);

    std::string line;
    while (std::getline(ss, line)) {
      LOG(INFO) << "evaluating: " << line;
      eval(read(line), &this->env);
    }

    LOG(INFO) << "enclave initialised";

    oak::InitialiseOutput out;
    return out;
  }

  oak::EvaluateOutput Evaluate(oak::EvaluateInput input) {
    oak::EvaluateOutput out;
    // TODO: Note that this modifies the env and therefore leaks sensitive data.
    out.set_output_data(to_string(eval(read(input.input_data()), &this->env)));
    return out;
  }
};

namespace asylo {

TrustedApplication *BuildTrustedApplication() {
  //return new EnclaveServer(absl::make_unique<::oak::grpc_server::OakServer>(),
      //asylo::EnclaveServerCredentials(asylo::BidirectionalNullCredentialsOptions()));
  return new EnclaveServer(absl::make_unique<::oak::grpc_server::OakServer>(),
      ::grpc::InsecureServerCredentials());
}

} // namespace asylo

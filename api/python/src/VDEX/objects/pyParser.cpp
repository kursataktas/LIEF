/* Copyright 2017 - 2023 R. Thomas
 * Copyright 2017 - 2023 Quarkslab
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
 */
#include "VDEX/pyVDEX.hpp"

#include "LIEF/VDEX/Parser.hpp"
#include "LIEF/VDEX/File.hpp"
#include "LIEF/logging.hpp"

#include "pyIOStream.hpp"
#include "pyutils.hpp"

#include <string>
#include <nanobind/stl/unique_ptr.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/string.h>

namespace LIEF::VDEX::py {

template<>
void create<Parser>(nb::module_& m) {
  using namespace LIEF::py;

  m.def("parse", nb::overload_cast<const std::string&>(&Parser::parse),
    "Parse the given filename and return a " RST_CLASS_REF(lief.VDEX.File) " object"_doc,
    "filename"_a, nb::rv_policy::take_ownership);

  m.def("parse",
      [] (nb::object obj, const std::string& name) {
        if (auto path_str = path_to_str(obj)) {
          return nb::cast(Parser::parse(std::move(*path_str)));
        }

        if (auto stream = PyIOStream::from_python(obj)) {
          auto ptr = std::make_unique<PyIOStream>(std::move(*stream));
          return nb::cast(Parser::parse(stream->content(), name));
        }
        logging::log(logging::LOG_ERR,
                     "LIEF parser interface does not support Python object: " +
                     type2str(obj));
        return nb::none();
      },
      "obj"_a, "name"_a = "",
      "parse(obj: io.IOBase | os.PathLike, name: str = ...) -> Optional[lief.VDEX.File]"_p,
      nb::rv_policy::take_ownership);
}
}

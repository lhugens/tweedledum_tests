Libraries used in tweedledum:
-----
abc - ABC: System for Sequential Logic Synthesis and Formal Verification
bill - C++ header-only reasoning library
Catch2 test framework for unit-tests, TDD and BDD
CMake for build automation
Eigen template library for linear algebra
{fmt} - A modern formatting library
kitty - truth table library
lorina - C++ parsing library for simple formats used in logic synthesis and formal verification
mockturtle - C++ logic network library
nlohmann/json - JSON for Modern C++
parallel_hashmap - A family of header-only, very fast and memory-friendly hashmap and btree containers.
percy - C++ header-only exact synthesis library
pybind11 - Seamless operability between C++11 and Python
rang - A Minimal, Header only Modern c++ library for terminal goodies

Relevant functions:

---------------------------------------------------------------
/tests/Synthesis/xag_synth.cpp

- function to create XAG network from a quantum circuit

```c++
mockturtle::xag_network to_xag_network(Circuit const& circuit, uint32_t num_i, uint32_t num_o)
```

- tests involve :

- creation of a XAG network named oracle:

```c++
auto oracle = mockturtle::xag_network()
auto a = oracle.create_pi();
auto b = oracle.create_pi();
auto ab = oracle.create_and(a, b);
oracle.create_po(ab);
```

- building a circuit from that XAG network using the xag_synth function:

```c++
Circuit circuit = xag_synth(oracle);

- turning it back to a XAG by using the helper function to_xag_network 

```c++
auto xag = to_xag_network(circuit, oracle.num_pis(), oracle.num_pos())
```

- and checking if they are equivalent.

---------------------------------------------------------------
/include/tweedledum/Synthesis/xag_synth.h

This is the header file included in /tests/Synthesis/xag_synth.cpp that has the xag_synth method.

```c++
namespace tweedledum {

void xag_synth(Circuit& circuit, std::vector<Qubit> const& qubits,
  std::vector<Cbit> const& cbits, mockturtle::xag_network const& xag,
  nlohmann::json const& config = {});

Circuit xag_synth(
  mockturtle::xag_network const& xag, nlohmann::json const& config = {});

} // namespace tweedledum
```

---------------------------------------------------------------
/python/tweedledum/classical/mockturtle.cpp

lorina is used to parse a dimacs file into a mockturtle::xag_newtork 

```c++
module.def("read_dimacs", [](std::string const filename) {
    xag_network xag;
    lorina::read_dimacs(filename, dimacs_reader(xag));
    return xag;
}, "Create a LogicNetwork from a DIMACS file.");
```

---------------------------------------------------------------
/external/mockturtle/mockturtle/io/dimacs_reader.hpp

Location of the dimacs reader used above

---------------------------------------------------------------
/external/lorina/lorina/dimacs.hpp

This file might be useful to create SAT instances without having to write them into a file.

Two methods of interest are:

```c++
[[nodiscard]] inline return_code read_dimacs(const std::string& filename, 
                                            const dimacs_reader& reader, 
                                            diagnostic_engine* diag = nullptr )
```
which uses a file as input, and

```c++
[[nodiscard]] inline return_code read_dimacs(std::istream& in,
                                            const dimacs_reader& reader,
                                            diagnostic_engine* diag = nullptr )
```

which uses a istream as input. The second may be the most useful since it may allow to pass dimacs instances witout creating the files themselves.

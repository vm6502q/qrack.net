# QrackNet Script API Reference

QrackNet API is open-source software to serve [unitaryfund/qrack](https://github.com/unitaryfund/qrack) gate-based quantum computer simulation "jobs" via a [PyQrack](https://github.com/unitaryfund/pyqrack)-like (lower-level) scripting language, as a Node.js-based web API. (Also see the [usage examples](https://github.com/vm6502q/qrack.net/blob/main/api/EXAMPLES.md), to help understand end-to-end workflows.

## Script API Routes

##### `POST /api/qrack`

Accepts a script definition for the web API server to run, and returns a "Job ID."

- `program`: Array of method "instructions," executed in order from the first


##### `GET /api/qrack/:jobId`

Returns the status and "**output space**" of the job. All methods that return any output write it to the job (global) "output space," with names specified by the user that become schema for the "`output`" object.

## Glossary

**All glossary type precision is less than or equal to 53-bit, from JavaScript `Number` type.**

- `bitLenInt`: Bit-length integer - unsigned integer ID of qubit position in register.
- `bitCapInt`: Bit-capacity integer - unsigned integer permutation basis eigenstate value of a qubit register (typically "big integer," limited in input by JavaScript `Number` type).
- `real1`: Real number (1-dimensional) - floating-point real-valued number. **(JSON input precision: 32-bit IEEE floating-point.)**
- `Pauli`: Enum for Pauli bases - X is `1`, Z is `2`, Y is `3`, and "identity" is `0`.
- `quid`: Quantum (simulator) unique identifier - unsigned integer that indexes and IDs running simulators and neurons.

## Methods

Each method, as a line of the `program` array argument of the `POST /api/qrack` route has the following fields:

- `name`: String with the method name of the script instruction (as listed below, exact match, without markdown)
- `parameters`: Array of method parameters **in same order as defined in this reference document**. (In other words, supply "parameters" as **according to position in list**, not by "property" name, according this reference document, below.) `quid` arguments are always specified as variable names from the job's live "output space" of global variables, not number "literals." For "semi-classical" boolean methods, like `cland` below, boolean parameters can be supplied as variable names from the output space.
- `output`: **This is only used and required if a method returns a value.** Gives a name to the output of this method in the job's "output space." (If the variable name already exists, it will be overwritten.)
- `program`: **This is only used and required if a method is a classical boolean control structure.** When an classical boolean variable control condition evaluates to `true`, this nested `program` property is immediately dispatched like a general top-level QrackNet script, as a conditional subroutine.

### Simulator Initialization

##### `init_general(bitLenInt length) -> quid`

**Returns** a `quid` representing a newly-initialized simulator optimized for "BQP-complete" (general) problems.

**As of the current pre-release version, prefer `init_qbdd` for general problems.** (`init_general` will ultimately serve this role, but its status is effectively experimental.)

- `length`: Number of qubits.


##### `init_stabilizer(bitLenInt length) -> quid`

**Returns** a `quid` representing a newly-initialized simulator optimized for ("hybrid") stabilizer problems (with recourse to universal circuit logic as a fallback).

- `length`: Number of qubits.


##### `init_qbdd(bitLenInt length) -> quid`

**Returns** a `quid` representing a newly-initialized simulator for low-entanglement problems (with "quantum binary decision diagrams" or "QBDD" simulation)

- `length`: Number of qubits.


##### `init_clone(quid sid) -> quid`

**Returns** a `quid` representing a newly-initialized clone of an existing simulator.

- `sid`: Simulator instance ID.


##### `destroy(quid sid)`

Destroys or releases a simulator instance.

- `sid`: Simulator instance ID.


### Random Number Generation

##### `seed(quid sid, unsigned s)`

Seeds the random number generator.

- `sid`: Simulator instance ID.
- `s`: Seed value.


### Qubit Management

##### `allocate_qubit(quid sid, bitLenbitLenInt qid)`

Allocates a new qubit with a specific ID.

- `sid`: Simulator instance ID.
- `qid`: Qubit ID.


##### `release_qubit(quid sid, bitLenInt q) -> bool`

**Returns** true if the qubit ID is 'zeroed', and releases the qubit in any case.

- `sid`: Simulator instance ID.
- `qid`: Qubit ID.


##### `num_qubits(quid sid) -> bitLenInt`

**Returns** the total count of qubits in a simulator instance.

- `sid`: Simulator instance ID.


### Measurement and Expectation Value Methods

##### `prob(quid sid, bitLenInt q) -> real1`

**Returns** The probability (from `0.0` to `1.0`) of the qubit being in the |1> state.

- `sid`: Simulator instance ID.
- `q`: Qubit ID.


##### `prob_rdm(quid sid, bitLenInt q) -> real1`

**Returns** a "best-guess" (for near-Clifford simulation) for probability of the qubit being in the |1> state, based on the "reduced density matrix" (with less overhead to calculate, for being "RDM").

- `sid`: Simulator instance ID.
- `q`: Qubit ID.


##### `perm_prob(quid sid, std::vector<bitLenInt> q, std::vector<Pauli> b) -> real1`

**Returns** the probability (upon measurement, in the corresponding joint Pauli basis) of collapsing into a specified permutation of a group of qubits.

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `b`: Array of Pauli axes (for each qubit ID in `q`).


##### `perm_prob_rdm(quid sid, std::vector<bitLenInt> q, std::vector<Pauli> b, bool r) -> real1`

**Returns** a "best-guess" (for near-Clifford simulation) as to the probability (upon measurement, in the corresponding joint Pauli basis) of collapsing into a specified permutation of a group of qubits, based on the "reduced density matrix" (with less overhead to calculate, for being "RDM").

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `b`: Array of Pauli axes (for each qubit ID in `q`).
- `r`: "Rounding" option on/off, for `true`/`false`.


##### `fact_exp(quid sid, std::vector<bitLenInt> q, std::vector<long> s) -> real1`

**Returns** an expectation value by summing respective integers "`s`", associated to |0> and |1> respective states of each qubit in "`q`", across qubit basis ray permutations by probability "weight," with `s` strided in |0>/|1> pairs, as a flat array.

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `s`: Array of integers (associated to respective |0> and |1> states of each qubit ID in `q`, with twice as many elements as `q`).


##### `fact_exp_rdm(quid sid, std::vector<bitLenInt> q, std::vector<long> s, bool r) -> real1`

**Returns** a "best-guess" (for near-Clifford simulation) expectation value based on the "reduced density matrix," for an expectation value resulting from summing respective integers "`s`", associated to |0> and |1> respective states of each qubit in "`q`", across qubit basis ray permutations by probability "weight," with `s` strided in |0>/|1> pairs, as a flat array (with less overhead to calculate, for being "RDM").

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `s`: Array of integers (associated to respective |0> and |1> states of each qubit ID in `q`, with twice as many elements as `q`).
- `r`: "Rounding" option on/off, for `true`/`false`.


##### `fact_exp_fp(quid sid, std::vector<bitLenInt> q, std::vector<double> s) -> real1`

**Returns** an expectation value by summing respective floating-point numbers "`s`", associated to |0> and |1> respective states of each qubit in "`q`", across qubit basis ray permutations by probability "weight," with `s` strided in |0>/|1> pairs, as a flat array.

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `s`: Array of integers (associated to respective |0> and |1> states of each qubit ID in `q`, with twice as many elements as `q`).


##### `fact_exp_fp_rdm(quid sid, std::vector<bitLenInt> q, std::vector<double> s, bool r) -> real1`

**Returns** a "best-guess" (for near-Clifford simulation) expectation value based on the "reduced density matrix," for an expectation value resulting from summing respective floating-point numbers "`s`", associated to |0> and |1> respective states of each qubit in "`q`", across qubit basis ray permutations by probability "weight," with `s` strided in |0>/|1> pairs, as a flat array (with less overhead to calculate, for being "RDM").

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `s`: Array of integers (associated to respective |0> and |1> states of each qubit ID in `q`, with twice as many elements as `q`).
- `r`: "Rounding" option on/off, for `true`/`false`.


##### `measure(quid sid, bitLenInt q) -> bool`

**Returns** a boolean (true for |1> and false for |0>) single-qubit measurement result, simulated according to the Born rules, collapsing the state.

- `sid`: Simulator instance ID.
- `q`: Qubit ID.


##### `force_measure(quid sid, bitLenInt q, bool r)`

Forces the measurement result of a single qubit (and so does not save it to the output space, from input). This is a pseudo-quantum operation. (However, quantum computers could similarly apply "post-selective" measurements, at exponential disadvantage compared to classical simulators.)

- `sid`: Simulator instance ID.
- `q`: Qubit ID.
- `r`: Desired measurement result to force.


##### `measure_basis(quid sid, std::vector<bitLenInt> q, std::vector<Pauli> b) -> bool`

**Returns** a single boolean measurement result upon collapse of an ensemble of qubits, jointly, via measurement, each in its specified Pauli basis.

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `b`: Array of Pauli axes (for each qubit ID in `q`).


##### `measure_all`(quid sid) -> bitCapInt`

**Returns** the bit string resulting from measuring all qubits according to the Born rules, collapsing the simulator state.

- `sid`: Simulator instance ID.


##### `measure_shots(quid sid, std::vector<bitLenInt> q, unsigned s) -> std::vector<bitCapInt>`

**Returns** an array of bit strings resulting from repeatedly measuring a set of qubits for a specified number of shots in the Z-basis, without collapsing the simulator state.

- `sid`: Simulator instance ID.
- `q`: Vector of qubit identifiers.
- `s`: Number of measurement shots.

##### `reset_all(quid sid)`

Resets the simulator state to the |0> permutation state for all qubits.

- `sid`: Simulator instance ID.


### Single-qubit gates

#### Discrete single-qubit gates

**Each gate below takes the same two parameters:**

- `sid`: Simulator instance ID.
- `q`: Qubit ID.

These are the gates:

- `x(quid sid, bitLenInt q)`
- `y(quid sid, bitLenInt q)`
- `z(quid sid, bitLenInt q)`
- `h(quid sid, bitLenInt q)`
- `s(quid sid, bitLenInt q)`
- `sx(quid sid, bitLenInt q)`
- `sy(quid sid, bitLenInt q)`
- `t(quid sid, bitLenInt q)`
- `adjs(quid sid, bitLenInt q)`
- `adjsx(quid sid, bitLenInt q)`
- `adjsy(quid sid, bitLenInt q)`
- `adjt(quid sid, bitLenInt q)`

#### Parameterized single-qubit gates

##### `u(quid sid, bitLenInt q, real1 theta, real1 phi, real1 lambda)`

General 3-parameter unitary single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator instance ID.
- `q`: Qubit ID.
- `theta`: angle (radians)
- `phi`: angle (radians)
- `lambda`: angle (radians)


##### `mtrx(quid sid, std::vector<double> m, bitLenInt q)`

General 2x2 unitary matrix operator single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator instance ID.
- `m`: 8 floating-point numbers in a "flat" array representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `r(quid sid, double phi, bitLenInt q, Pauli b)`

Rotates qubit by the specified angle around the specified Pauli axis.

- `sid`: Simulator instance ID.
- `phi`: Angle of rotation in radians.
- `q`: Qubit ID.
- `b`: Pauli axis of rotation.


### Multi-controlled single-qubit gates

**"MC"** gates are activated by **|1>** control qubit states.
**"MAC"** gates are activated by **|0>** control qubit states.

#### Discrete multi-controlled single-qubit gates

**Each gate below takes the same three parameters:**

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Qubit ID.

These are the gates:

- `mcx(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mcy(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mcz(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mch(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mcs(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mct(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mcadjs(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mcadjt(quid sid, std::vector<bitLenInt> c, bitLenInt q)`

- `macx(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `macy(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `macz(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mach(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `macs(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `mact(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `macadjs(quid sid, std::vector<bitLenInt> c, bitLenInt q)`
- `macadjt(quid sid, std::vector<bitLenInt> c, bitLenInt q)`

#### Parameterized mult-controlled single-qubit gates

##### `mcu(quid sid, std::vector<bitLenInt> c, bitLenInt q, real1 theta, real1 phi, real1 lambda)`
##### `macu(quid sid, std::vector<bitLenInt> c, bitLenInt q, real1 theta, real1 phi, real1 lambda)`

General 3-parameter unitary single-qubit target with arbitrary number of control qubits (covers all possible single-qubit target "payloads")

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Qubit ID.
- `theta`: angle
- `phi`: angle
- `lambda`: angle


##### `mcmtrx(quid sid, std::vector<bitLenInt> c, std::vector<double> m, bitLenInt q)`
##### `macmtrx(quid sid, std::vector<bitLenInt> c, std::vector<double> m, bitLenInt q)`

General 2x2 unitary matrix operator single-qubit target with arbitrary number of control qubits (covers all possible single-qubit target "payloads")

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `m`: 8 floating-point numbers representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `mcr(quid sid, std::vector<bitLenInt> c, double phi, bitLenInt q, Pauli b)`

Rotates qubit by the specified angle around the specified Pauli axis.

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `phi`: Angle of rotation in radians.
- `q`: Qubit ID.
- `b`: Pauli axis of rotation.


#### Special multi-controlled single-qubit gates

##### `ucmtrx(quid sid, std::vector<bitLenInt> c, std::vector<real1> m, bitLenInt q, bitLenInt p)`

Multi-controlled gate that activates only for the specified permutation of controls

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `m`: 8 floating-point numbers in a "flat" array representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `multiplex_1qb_mtrx(quid sid, std::vector<bitLenInt> c, std::vector<real1> m, bitLenInt q)`

Multi-controlled, single-target multiplexer gate

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `m`: For **each** permutation of control qubits, numbered from 0 as ascending binary (unsigned) integers, in an overall "flat" array, 8 floating-point numbers representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


### Coalesced single-qubit gates

**These optimized gates apply the same Pauli operator to all specified qubits and take the same two arguments. This is optimized, compared to separate Pauli gates.**

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.

These are the gates:

- `mx(quid sid, std::vector<bitLenInt> q)`
- `my(quid sid, std::vector<bitLenInt> q)`
- `mz(quid sid, std::vector<bitLenInt> q)`


### Multi-qubit Pauli exponentiation gates

##### `exp(quid sid, std::vector<bitLenInt> q, std::vector<Pauli> b, real1 phi)`

Applies e^{-i * theta * b}, exponentiation of the specified Pauli operator corresponding to each qubit

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.
- `phi`: Angle of the rotation in radians.


##### `mcexp(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<Pauli> b, real1 phi)`

Applies e^{-i * theta * b}, exponentiation of the specified Pauli operator corresponding to each qubit

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of qubit IDs.
- `phi`: Angle of the rotation in radians.

### Swap gate variants

##### `swap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits

- `sid`: Simulator instance ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `iswap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits and apply a factor of "i" if their states differ

- `sid`: Simulator instance ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `adjiswap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits and apply a factor of "-i" if their states differ (inverse of `iswap`)

- `sid`: Simulator instance ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `fsim(quid sid, real1 theta, real1 phi bitLenInt q1, bitLenInt q2)`

Apply "fsim," which is a phased swap-like gate that is useful in fermionic simulation

- `sid`: Simulator instance ID.
- `theta`: angle (radians) (1)
- `phi`: angle (radians) (2)
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `mcswap(quid sid, std::vector<bitLenInt> c, bitLenInt q1, bitLenInt q2)`

If controls are all |1>, swap the two input qubits

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `macswap(quid sid, std::vector<bitLenInt> c, bitLenInt q1, bitLenInt q2)`

If controls are all |0>, swap the two input qubits

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


### Boolean (Toffoli) gates

**Each gate below takes the same four parameters:**

- `sid`: Simulator instance ID.
- `qi1`: Input qubit ID (1).
- `qi2`: Input qubit ID (2).
- `qo`: Output qubit ID.

These are the gates:

- `and(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`
- `or(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`
- `xor(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`
- `nand(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`
- `nor(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`
- `xnor(quid sid, bitLenInt qi1, bitLenInt qi2, bitLenInt qo)`

### Boolean (Semi-Classical) gates

**Each gate below takes the same four parameters:**

- `sid`: Simulator instance ID.
- `ci`: Input classical bit value (literal value or boolean output space variable name).
- `qi`: Input qubit ID.
- `qo`: Output qubit ID.

These are the gates:

- `cland(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clxor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clnand(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clnor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clxnor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`

**Boolean parameters in the methods above can be specified as either values or output space variable names.**

There is a special method for saving `true` or `false` to an ouput method variable:

##### `write_bool(bool b) -> bool`

Write a boolean value to the output space, with a variable name

- `b`: Bool to write to `output` variable name.


### Quantum Fourier transform

##### `qft(quid sid, std::vector<bitLenInt> q)`

Acts the quantum Fourier transform on the specified set of qubits (without terminal swap gates to reverse bit order)

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.


##### `iqft(quid sid, std::vector<bitLenInt> q)`

Acts the inverse of the quantum Fourier transform on the specified set of qubits (without terminal swap gates to reverse bit order)

- `sid`: Simulator instance ID.
- `q`: Array of qubit IDs.

### Arithmetic Logic Unit (ALU)

##### `add(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Add classical integer to quantum integer (in-place)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `sub(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Subtract classical integer from quantum integer (in-place)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `adds(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Add classical integer to quantum integer (in-place) and set an overflow flag

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.
- `s`: Qubit ID of overflow flag.


##### `subs(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Subtract classical integer from quantum integer (in-place) and set an overflow flag

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.
- `s`: Qubit ID of overflow flag.


##### `mcadd(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

If controls are all |1>, add classical integer to quantum integer (in-place)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `mcsub(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

If controls are all |1>, subtract classical integer from quantum integer (in-place)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `mul(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

Multiply quantum integer by classical integer (in-place)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `div(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Divide quantum integer by classical integer (in-place)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `muln(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Multiply quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `divn(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Divide quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `pown(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Raise a classical base to a quantum power (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcmul(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

If controls are all |1>, multiply quantum integer by classical integer (in-place)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `mcdiv(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

If controls are all |1>, divide quantum integer by classical integer (in-place)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `mcmuln(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, multiply quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcmuln(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, divide quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcpown(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, raise a classical base to a quantum power (out-of-place, with modulus)

- `sid`: Simulator instance ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


## Quantum Neuron Activation Functions

Quantum neurons can use different activation functions, as defined in the QNeuronActivationFn enumeration:

- `Sigmoid (Default)`: Standard sigmoid activation function.
- `ReLU`: Rectified Linear Unit activation function.
- `GeLU`: Gaussian Error Linear Unit activation function.
- `Generalized Logistic`: A variation of the sigmoid function with tunable sharpness.
- `Leaky ReLU`: Leaky version of the Rectified Linear Unit activation function.

## Quantum Neuron Methods

### Initialization

##### `init_qneuron(quid sid, std::vector<bitLenInt> c, bitLenInt q, QNeuronActivationFn f, real1 a, real1 tol) -> quid`

Initializes a quantum neuron with specified parameters.

- `sid`: Simulator instance ID.
- `c`: List of control qubits for input.
- `q`: Target qubit for output.
- `f`: Activation function.
- `a`: Alpha parameter (specific to certain activation functions).
- `tol`: Tolerance for neuron activation.

### Cloning

##### `clone_qneuron(quid nid) -> quid`

Clones an existing quantum neuron.

- `nid`: Neuron instance ID.

### Destruction

##### `destroy_qneuron(quid nid)`

Destroys a quantum neuron.

- `nid`: Neuron instance ID.

### Configuration

##### `set_qneuron_angles(quid nid, std::vector<real1> angles)`

Sets the RY-rotation angle parameters for the quantum neuron.

- `nid`: Neuron instance ID.
- `angles`: Vector of angles for each input permutation.


##### `get_qneuron_angles(quid nid) -> std::vector<real1>`

Retrieves the RY-rotation angle parameters of the quantum neuron.

- `nid`: Neuron instance ID.
- `Returns`: Vector of angles.


##### `set_qneuron_alpha(quid nid, real1 alpha)`

Sets the leakage parameter for leaky quantum neuron activation functions.

- `nid`: Neuron instance ID.
- `alpha`: Leakage parameter value.


##### `set_qneuron_activation_fn(quid nid, QNeuronActivationFn f)`

Sets the activation function of a quantum neuron.

- `nid`: Neuron instance ID.
- `f`: Activation function.

### Learning and Inference

##### `qneuron_predict(quid nid, bool e, bool r) -> real1`

**Returns** an inference result using the quantum neuron.

- `nid`: Neuron instance ID.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/leave the output qubit state before inference


##### `qneuron_unpredict(quid nid, bool e) -> real1`

**Returns** an inference result using the inverse operation of neuron inference.

- `nid`: Neuron instance ID.
- `e`: Expected boolean inference result.


##### `qneuron_learn_cycle(quid nid, bool e) -> real1`

**Returns** an inference result using the quantum neuron, training for one epoch and uncomputing intermediate effects.

- `nid`: Neuron instance ID.
- `e`: Expected boolean inference result.


##### `qneuron_learn(quid nid, real1 eta, bool e, bool r)`

Trains the quantum neuron for one epoch.

- `nid`: Neuron instance ID.
- `eta`: Learning rate.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/keep the output qubit state before learning


##### `qneuron_learn_permutation(quid nid, real1 eta, bool e, bool r)`

Trains the quantum neuron for one epoch, assuming a Z-basis eigenstate input.

- `nid`: Neuron instance ID.
- `eta`: Learning rate.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/keep the output qubit state before learning

### Schmidt Decomposition Rounding Parameter and Near-Clifford Rounding (Approximation)

Fidelity methods are strictly `double` precision, not explicitly `real1`.

##### `set_sdrp(quid sid, double sdrp)`

Set the "Schmidt decomposition rounding parameter" ("SDRP"). If "reactive separation" option is on (as by default) the parameter will be automatically applied in multi-qubit gate operations. (See [arXiv:2304.14969](https://arxiv.org/abs/2304.14969), by Strano and the Qrack and Unitary Fund teams, on comparative benchmarks relative to Qrack.)

- `sid`: Simulator instance ID.
- `sdrp`: Schmidt decomposition rounding parameter (0 to 1 range, defaults to real1 "epsilon")


##### `set_ncrp(quid sid, double ncrp)`

Set the "near-Clifford rounding parameter" ("NCRP"). When near-Clifford gate set is being used (such as general Clifford gates plus general single-qubit phase gates), this value controls how "severe" any non-Clifford effect needs to be, to be taken into consideration, or else (internally managed and applied) non-Clifford gates might be ignored.

- `sid`: Simulator instance ID.
- `ncrp`: Near-Clifford rounding parameter (0 to 1 range, defaults to real1 "epsilon")


##### `get_unitary_fidelity(quid sid) -> double`

Report a close theoretical estimate of fidelity, as potentially reduced by "SDRP" (Credit to Andrea Mari for research at Unitary Fund, in [arXiv:2304.14969](https://arxiv.org/abs/2304.14969))

- `sid`: Simulator instance ID.
- `sdrp`: Schmidt decomposition rounding parameter (0 to 1 range, defaults to real1 "epsilon")


##### `reset_unitary_fidelity(quid sid)`

Reset the "SDRP" fidelity tracker to 1.0 fidelity ("ideal"), before continuing fidelity calculation. (Some wholly-destructive measurement and state preparation operations and side effects might automatically reset the fidelity tracker to 1.0, as well, though the attempt in design is to do so unobstructively to the utility of the fidelity tracking function in typical use.)

- `sid`: Simulator instance ID.


##### `set_reactive_separate(quid sid)`

Turn "reactive separation" optimization on/off with true/false (default: on/true). (Some subjectively "high-entanglement" circuits will run more quickly with "reactive separation" off.)

- `sid`: Simulator instance ID.


##### `set_t_injection(quid sid)`

Turn "near-Clifford" simulation techniques (for not just "`t`" gate, but "`r`" around Pauli Z axis in general) on/off, with true/false (default: on/true). (Near-clifford techniques are memory-efficient but might take very much longer execution time, without any "rounding" approximations applied, than other simulation techniques like state vector.)

- `sid`: Simulator instance ID.

### Classical control

These methods modify or base control upon boolean variables in the output space.

##### `not(bool b)`

Applies an in-place "not" operation to a boolean variable named by `b` in the output space (so `true` becomes `false`, and `false` becomes `true`).

- `b`: Boolean variable name.

##### `cif(bool b)`

**Dispatches** the additional `program` property of the method object as a subroutine if the boolean variable in the output space named by `b` is `true`.

- `b`: Boolean variable name.

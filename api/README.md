# QrackNet Script API Reference

QrackNet API is open-source software to serve [unitaryfund/qrack](https://github.com/unitaryfund/qrack) gate-based quantum computer simulation "jobs" via a [PyQrack](https://github.com/unitaryfund/pyqrack)-like (lower-level) scripting language, as a Node.js-based RESTful API on the web. (Also see the [usage examples](https://github.com/vm6502q/qrack.net/blob/main/api/EXAMPLES.md), to help understand end-to-end workflows.)

## Account API Routes

`POST /api/register`

If the request is valid and the username is not already in use, create an account and set an authentication cookie

- `username`: Account username.
- `affiliation`: User professional or institutional affiliation
- `name`: User personal name
- `email`: User email address
- `password`: Account password.
- `passwordConfirm`: Must match `password`


`PUT /api/login`

If the username and password are correct, set an authentication cookie

- `username`: Account username.
- `password`: Account password.


## Script API Routes

##### `POST /api/qrack`

Accepts a script definition for the RESTful API server to run, and returns a "Job ID."

- `program`: Array of method "instructions," executed in order from the first


##### `GET /api/qrack/:jobId`

Returns the status and "**output space**" of the job. All methods that return any output write it to the "output space," with a name specified by the user.

## Glossary

- `bitLenInt`: Bit-length integer - unsigned integer ID of qubit position in register.
- `bitCapInt`: Bit-capacity integer - unsigned integer single-permutation value of a qubit register (typically "big integer").
- `real1`: Real number (1-dimensional) - floating-point real-valued number.
- `Pauli`: Enum for Pauli bases - X is 1, Z is 2, Y is 3, and "identity" is 0.
- `quid`: Quantum (simulator) unique identifier - unsigned integer that indexes and IDs running simulators and neurons.

## Methods

Each method, as a line of the `program` array argument of the `POST /api/qrack` route has the following fields:

- `name`: String with the method name of the script instruction (as listed below, exact match)
- `parameters`: Array of method parameters **in same order as defined in this reference document**. `quid` arguments are always specified as variables in the "output space" of global variables.
- `output`: **This is only used and required if a method returns a value.** Gives a name to the output of this method in the "output space." (If the variable name already exists, it will be overwritten.)

### Simulator Initialization

##### `init_general(bitLenInt length)`

Initializes a simulator optimized for "BQP-complete" (general) problems.
**Returns** a quid representing the simulator created.

- `length`: Number of qubits.


##### `init_stabilizer(bitLenInt length)`

Initializes a simulator optimized for ("hybrid") stabilizer problems (with recourse to universal circuit logic as a fallback).
**Returns** a quid representing the simulator created.

- `length`: Number of qubits.


##### `init_qbdd(bitLenInt length)`

Initializes a simulator optimized for low-entanglement problems (with "quantum binary decision diagrams" or "QBDD" simulation)
**Returns** a quid representing the simulator created.

- `length`: Number of qubits.


##### `init_clone(quid sid)`

Clones a simulator instance.
**Returns** a quid representing the simulator created.

- `sid`: Simulator ID.


##### `destroy(quid sid)`

Destroys or releases a simulator instance.

- `sid`: Simulator ID.


### Random Number Generation and Concurrency

##### `seed(quid sid, unsigned s)`

Seeds the random number generator.

- `sid`: Simulator ID.
- `s`: Seed value.


##### `set_concurrency(quid sid, unsigned p)`

Sets CPU concurrency.

- `sid`: Simulator ID.
- `p`: Number of parallel threads.

### Qubit Management

##### `allocate_qubit(quid sid, bitLenbitLenInt qid)`

Allocates a new qubit with a specific ID.

- `sid`: Simulator ID.
- `qid`: Qubit ID.


##### `release_qubit(quid sid, bitLenInt q)`

Releases a qubit ID.

- `sid`: Simulator ID.
- `qid`: Qubit ID.


##### `num_qubits(quid sid)`

**Returns** the total count of qubits in a simulator instance.

- `sid`: Simulator ID.


### Quantum Probability and Measurement

##### `prob(quid sid, bitLenInt q)`

**Returns** the Z-basis expectation value of a qubit.

- `sid`: Simulator ID.
- `q`: Qubit ID.


##### `prob_rdm(quid sid, bitLenInt q)`

**Returns** a "best-guess" as to Z-basis expectation value of a qubit (for near-Clifford simulation) based on the "reduced density matrix" (with less overhead to calculate).

- `sid`: Simulator ID.
- `q`: Qubit ID.


### Single-qubit gates

#### Discrete single-qubit gates

**Each gate below takes the same two parameters:**

- `sid`: Simulator ID.
- `q`: Qubit ID.

These are the gates:

- `x(quid sid, bitLenInt q)`
- `y(quid sid, bitLenInt q)`
- `z(quid sid, bitLenInt q)`
- `h(quid sid, bitLenInt q)`
- `s(quid sid, bitLenInt q)`
- `t(quid sid, bitLenInt q)`
- `adjs(quid sid, bitLenInt q)`
- `adjt(quid sid, bitLenInt q)`

#### Parameterized single-qubit gates

##### `u(quid sid, bitLenInt q, real1 theta, real1 phi, real1 lambda)`

General 3-parameter unitary single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator ID.
- `q`: Qubit ID.
- `theta`: angle (radians)
- `phi`: angle (radians)
- `lambda`: angle (radians)


##### `mtrx(quid sid, std::vector<double> m, bitLenInt q)`

General 3-parameter unitary single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator ID.
- `m`: 8 floating-point numbers in a "flat" array representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `r(quid sid, double phi, bitLenInt q, Pauli b)`

Rotates qubit by the specified angle around the specified Pauli axis.

- `sid`: Simulator ID.
- `phi`: Angle of rotation in radians.
- `q`: Qubit ID.
- `b`: Pauli axis of rotation.


### Multi-controlled single-qubit gates

**"MC"** gates are activated by **|1>** control qubit states.
**"MAC"** gates are activated by **|0>** control qubit states.

#### Discrete multi-controlled single-qubit gates

**Each gate below takes the same three parameters:**

- `sid`: Simulator ID.
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

General 3-parameter unitary single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Qubit ID.
- `theta`: angle
- `phi`: angle
- `lambda`: angle


##### `mcmtrx(quid sid,  std::vector<bitLenInt> c, std::vector<double> m, bitLenInt q, )`
##### `macmtrx(quid sid,  std::vector<bitLenInt> c, std::vector<double> m, bitLenInt q, )`

General 3-parameter unitary single-qubit gate (covers all possible single-qubit gates)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `m`: 8 floating-point numbers representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `mcr(quid sid, std::vector<bitLenInt> c, double phi, bitLenInt q, Pauli b)`

Rotates qubit by the specified angle around the specified Pauli axis.

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `phi`: Angle of rotation in radians.
- `q`: Qubit ID.
- `b`: Pauli axis of rotation.


#### Special multi-controlled single-qubit gates

##### `ucmtrx(quid sid, std::vector<bitLenInt> c, std::vector<real1> m, bitLenInt q, bitLenInt p)`

Multi-controlled gate that activates only for the specified permutation of controls

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `m`: 8 floating-point numbers in a "flat" array representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


##### `multiplex_1qb_mtrx(quid sid, std::vector<bitLenInt> c, std::vector<real1> m, bitLenInt q)`

Multi-controlled, single-target multiplexer gate

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `m`: For **each** permutation of control qubits, numbered from 0 as ascending binary (unsigned) integers, in an overall "flat" array, 8 floating-point numbers representating alternating real/imaginary components of a (row-major) 2x2 complex unitary matrix.
- `q`: Qubit ID.


### Coalesced single-qubit gates

**These optimized gates apply the same Pauli operator to all specified qubits and take the same two arguments.**

- `sid`: Simulator ID.
- `q`: Array of qubit IDs.

These are the gates:

- `mx(quid sid, std::vector<bitLenInt> q)`
- `my(quid sid, std::vector<bitLenInt> q)`
- `mz(quid sid, std::vector<bitLenInt> q)`


### Multi-qubit Pauli exponentiation gates

##### `exp(quid sid, std::vector<bitLenInt> q, std::vector<Pauli> b, real1 phi)`

Applies e^{-i * theta * b}, exponentiation of the specified Pauli operator corresponding to each qubit

- `sid`: Simulator ID.
- `q`: Array of qubit IDs.
- `phi`: Angle of the rotation in radians.


##### `mcexp(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<Pauli> b, real1 phi)`

Applies e^{-i * theta * b}, exponentiation of the specified Pauli operator corresponding to each qubit

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of qubit IDs.
- `phi`: Angle of the rotation in radians.

### Swap gate variants

##### `swap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits

- `sid`: Simulator ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `iswap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits and apply a factor of "i" if their states differ

- `sid`: Simulator ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `adjiswap(quid sid, bitLenInt q1, bitLenInt q2)`

Swap the two input qubits and apply a factor of "-i" if their states differ (inverse of `iswap`)

- `sid`: Simulator ID.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `fsim(quid sid, real1 theta, real1 phi bitLenInt q1, bitLenInt q2)`

Apply "fsim," which is a phased swap-like gate that is useful in fermionic simulation

- `sid`: Simulator ID.
- `theta`: angle (radians) (1)
- `phi`: angle (radians) (2)
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `mcswap(quid sid, std::vector<bitLenInt> c, bitLenInt q1, bitLenInt q2)`

If controls are all |1>, swap the two input qubits

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


##### `macswap(quid sid, std::vector<bitLenInt> c, bitLenInt q1, bitLenInt q2)`

If controls are all |0>, swap the two input qubits

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q1`: Qubit ID (1).
- `q2`: Qubit ID (2).


### Boolean (Toffoli) gates

**Each gate below takes the same four parameters:**

- `sid`: Simulator ID.
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

- `sid`: Simulator ID.
- `ci`: Input classical bit value.
- `qi`: Input qubit ID.
- `qo`: Output qubit ID.

These are the gates:

- `cland(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clxor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clnand(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clnor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`
- `clxnor(quid sid, bool ci, bitLenInt qi, bitLenInt qo)`

### Quantum Fourier transform

##### `qft(quid sid, std::vector<bitLenInt> q)`

Acts the quantum Fourier transform on the specified set of qubits (without terminal swap gates to reverse bit order)

- `sid`: Simulator ID.
- `q`: Array of qubit IDs.


##### `iqft(quid sid, std::vector<bitLenInt> q)`

Acts the inverse of the quantum Fourier transform on the specified set of qubits (without terminal swap gates to reverse bit order)

- `sid`: Simulator ID.
- `q`: Array of qubit IDs.

### Arithmetic Logic Unit (ALU)

##### `add(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Add classical integer to quantum integer (in-place)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `sub(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Subtract classical integer from quantum integer (in-place)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `adds(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Add classical integer to quantum integer (in-place) and set an overflow flag

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.
- `s`: Qubit ID of overflow flag.


##### `subs(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Subtract classical integer from quantum integer (in-place) and set an overflow flag

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.
- `s`: Qubit ID of overflow flag.


##### `mcadd(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

If controls are all |1>, add classical integer to quantum integer (in-place)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `mcsub(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

If controls are all |1>, subtract classical integer from quantum integer (in-place)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `a`: Classical integer operand.


##### `mul(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

Multiply quantum integer by classical integer (in-place)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `div(quid sid, std::vector<bitLenInt> q, bitCapInt a)`

Divide quantum integer by classical integer (in-place)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `muln(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Multiply quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `divn(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Divide quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `pown(quid sid, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

Raise a classical base to a quantum power (out-of-place, with modulus)

- `sid`: Simulator ID.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcmul(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

If controls are all |1>, multiply quantum integer by classical integer (in-place)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `mcdiv(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a)`

If controls are all |1>, divide quantum integer by classical integer (in-place)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of overflow qubit IDs.
- `a`: Classical integer operand.


##### `mcmuln(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, multiply quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcmuln(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, divide quantum integer by classical integer (out-of-place, with modulus)

- `sid`: Simulator ID.
- `c`: Array of control qubit IDs.
- `q`: Array of target qubit IDs.
- `o`: Array of output qubit IDs.
- `a`: Classical integer operand.
- `m`: Modulo base.


##### `mcpown(quid sid, std::vector<bitLenInt> c, std::vector<bitLenInt> q, std::vector<bitLenInt> o, bitCapInt a, bitCapInt m)`

If controls are all |1>, raise a classical base to a quantum power (out-of-place, with modulus)

- `sid`: Simulator ID.
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

##### `init_qneuron(quid sid, std::vector<bitLenInt> c, bitLenInt q, QNeuronActivationFn f, real1 a, real1 tol)`

Initializes a quantum neuron with specified parameters.

- `sid`: Simulator ID.
- `c`: List of control qubits for input.
- `q`: Target qubit for output.
- `f`: Activation function.
- `a`: Alpha parameter (specific to certain activation functions).
- `tol`: Tolerance for neuron activation.

### Cloning

##### `clone_qneuron(quid nid)`

Clones an existing quantum neuron.

- `nid`: Neuron ID.

### Destruction

##### `destroy_qneuron(quid nid)`

Destroys a quantum neuron.

- `nid`: Neuron ID.

### Configuration

##### `set_qneuron_angles(quid nid, std::vector<real1> angles)`

Sets the RY-rotation angle parameters for the quantum neuron.

- `nid`: Neuron ID.
- `angles`: Vector of angles for each input permutation.


##### `get_qneuron_angles(quid nid)`

Retrieves the RY-rotation angle parameters of the quantum neuron.

- `nid`: Neuron ID.
- `Returns`: Vector of angles.


##### `set_qneuron_alpha(quid nid, real1 alpha)`

Sets the leakage parameter for leaky quantum neuron activation functions.

- `nid`: Neuron ID.
- `alpha`: Leakage parameter value.


##### `set_qneuron_activation_fn(quid nid, QNeuronActivationFn f)`

Sets the activation function of a quantum neuron.

- `nid`: Neuron ID.
- `f`: Activation function.

### Learning and Inference

##### `qneuron_predict(quid nid, bool e, bool r)`

**Returns** an inference result using the quantum neuron.

- `nid`: Neuron ID.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/leave the output qubit state before inference


##### `qneuron_unpredict(quid nid, bool e)`

**Returns** an inference result using the inverse operation of neuron inference.

- `nid`: Neuron ID.
- `e`: Expected boolean inference result.


##### `qneuron_learn_cycle(quid nid, bool e)`

**Returns** an inference result using the quantum neuron, training for one epoch and uncomputing intermediate effects.

- `nid`: Neuron ID.
- `e`: Expected boolean inference result.


##### `qneuron_learn(quid nid, real1 eta, bool e, bool r)`

Trains the quantum neuron for one epoch.

- `nid`: Neuron ID.
- `eta`: Learning rate.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/keep the output qubit state before learning


##### `qneuron_learn_permutation(quid nid, real1 eta, bool e, bool r)`

Trains the quantum neuron for one epoch, assuming a Z-basis eigenstate input.

- `nid`: Neuron ID.
- `eta`: Learning rate.
- `e`: Expected boolean inference result.
- `r`: Boolean to reset/keep the output qubit state before learning

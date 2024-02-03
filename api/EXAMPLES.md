# QrackNet Usage Examples

This guide assumes authenticated access to QrackNet, holding a necessary session cookie if required. (Sometimes a session cookie is not required.)

## Flipping a (Simulated) Quantum Coin

Simulate a quantum coin flip using a single qubit in superposition. Use the `POST /api/qrack` route with the script below. It initializes a one-qubit simulator, applies a Hadamard gate, and measures the qubit. The first operation creates a simulator instance (`qsim`), which is then used in subsequent operations. Qubits are indexed starting at 0.

```json
{
    "program": [
        { "name": "init_general", "parameters": [1], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "measure", "parameters": ["qsim", 0], "output": "result" }
    ]
}
```

Upon submission, a response with a unique job ID is received. Later, retrieve the job's outcome (true or false) using `GET /api/qrack/{jobId}`.

## Flipping Two Entangled "Coins"

To demonstrate entanglement, we use a Bell pair. This script initializes a two-qubit simulator, applies a Hadamard gate to the first qubit, entangles them using a CNOT gate, and measures the entangled state using multiple shots.

```json
{
    "program": [
        { "name": "init_general", "parameters": [2], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1], 8], "output": "result" }
    ]
}
```

Submit the script and use the returned job ID to check the results. The outcomes should demonstrate correlated states of the qubits, reflecting entanglement.

## Quantum teleportation

It is particularly instructive to see a script that can simulate the "quantum teleportation" algorithm, in QrackNet syntax:
```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [3], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "mcx", "parameters": ["qsim", [1], 2] },
        { "name": "u", "parameters": ["qsim", 0, 0.3, 2.2, 1.4] },
        { "name": "prob", "parameters": ["qsim", 0], "output": "aliceZ" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "prob", "parameters": ["qsim", 0], "output": "aliceX" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "measure", "parameters": ["qsim", 0], "output": "aliceM0" },
        { "name": "measure", "parameters": ["qsim", 1], "output": "aliceM1" },
        { "name": "cif", "parameters": ["aliceM0"], "program": { "name": "z", "parameters": ["qsim", 2] }},
        { "name": "cif", "parameters": ["aliceM1"], "program": { "name": "x", "parameters": ["qsim", 2] }},
        { "name": "prob", "parameters": ["qsim", 2], "output": "bobZ" },
        { "name": "h", "parameters": ["qsim", 2] },
        { "name": "prob", "parameters": ["qsim", 2], "output": "bobX" }
    ]
}
```
"Alice" and "Bob" each have different qubits in a Bell pair; Alice entangles a (1-qubit) "message" to send to Bob (characterized by `"aliceZ"` and `"aliceX"`); Alice measures her two qubits and "sends" the classical bit results to Bob; Bob acts up to two classically-controlled operations on his remaining Bell pair qubit, to "receive" the message (in `"bobZ"` and `"bobX"`). Refer to any good textbook or [encyclopedia](https://en.wikipedia.org/wiki/Quantum_teleportation) to further "unpack" and explain the program operations used above, though we provide it as an authoritative reference implementation in QrackNet script specifically, of the common thought experiment where "Alice" transmits a message to "Bob" with a Bell pair.

`"aliceZ"` and `"aliceX"` are the state of the original "message" qubit to teleport, and they should match the `"bobZ"` and `"bobX"` message received by the end of the algorithm and program in the job results:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "aliceZ": 0.022331755608320236,
            "aliceX": 0.5869569778442383,
            "aliceM0": true,
            "aliceM1": true,
            "bobZ": 0.022331759333610535,
            "bobX": 0.5869571566581726
        }
    }
}
```

## Grover's search algorithm (AKA "amplitude amplification")

Encoding "oracles" to "solve" with Grover's search algorithm (or "amplitude amplification") is relatively easy with use of the QrackNet script's "arithmetic logic unit" ("ALU") methods. We initialize a simulator, then we prepare it in initial maximal superposition, then the third code section below is the "oracle" we are trying to "invert" (or "search") using the advantaged quantum algorithm. The oracle tags only the bit string "`3`" as a "match." Then, according to the general amplitude amplification algorithm, we "uncompute" and reverse phase on the original input state, re-prepare with `h` gates, and iterate for `floor(sqrt(pow(2, n)) * PI / 4)` total repetitions to search an oracle of `n` qubits width with a single accepted "match" to the oracle:

```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [3], "output": "qsim" },

        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },

        { "name": "sub", "parameters": ["qsim", [0, 1, 2], 3] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "macz", "parameters": ["qsim", [0, 1], 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "add", "parameters": ["qsim", [0, 1, 2], 3] },

        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "macz", "parameters": ["qsim", [0, 1], 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },

        { "name": "sub", "parameters": ["qsim", [0, 1, 2], 3] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "macz", "parameters": ["qsim", [0, 1], 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "add", "parameters": ["qsim", [0, 1, 2], 3] },

        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "macz", "parameters": ["qsim", [0, 1], 2] },
        { "name": "x", "parameters": ["qsim", 2] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },

        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2], 8], "output": "result" }
    ]
}
```

Surely enough, our job response will look like this (with a typically very small probabilistic element to the measurement "shot" output distribution):
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "result": [
                3,
                3,
                3,
                3,
                3,
                3,
                3,
                3
            ]
        }
    }
}
```

## GHZ-state preparation

The QrackNet server is likely to successfully run even 50-qubit or larger GHZ state preparation circuits (with measurement), though reporting precision for results might currently be limited to about 32 bits. A 50-qubit GHZ-state preparation definitely won't work with `init_general` (which will ultimately attempt to allocate ~50 qubits of state vector representation) but it's likely to work with `init_stabilizer` and `init_qbdd`, as both are optimized (differently) for this GHZ-state case.

We can think of the state preparation as an `h` gate followed by a loop over `mcx` gates with single control qubits, but we need to "manually unroll" the conceptual loop (for 4 qubits in total, for this example):
```json
{
    "program" : [
        { "name": "init_stabilizer", "parameters": [4], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "mcx", "parameters": ["qsim", [0], 2] },
        { "name": "mcx", "parameters": ["qsim", [0], 3] },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2, 3], 16], "output": "result" }
    ]
}
```
If we exceed Clifford gate set "vocabulary" in any single line of the script, we are likely (but not guaranteed) to fall back to state vector with `init_stabilizer`, which in a case like with 50 qubits will lead to a (self-recovering, handled) "server crash." On the other hand, `init_qbdd` can also efficiently handle this state preparation without the requirement to constrain one's gate set vocabulary to pure Clifford gates. (This might make QBDD the more powerful option in many cases where GHZ-state preparation is only one "subroutine" in a larger quantum algorithm with a universal gate set.)

This an example result, for the above job:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "result": [
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                15,
                15,
                15,
                15,
                15,
                15,
                15,
                15
            ]
        }
    }
}
```

## Quantum Fourier transform

The "quantum Fourier transform" ("QFT") is exposed as a complete subroutine via a single method call:
```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [3], "output": "qsim" },
        { "name": "qft", "parameters": ["qsim", [0, 1, 2]] },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2], 8], "output": "result" }
    ]
}
```
`iqft` instruction is similarly the "inverse QFT", with the same syntax. These method do **not** apply terminal or leading `swap` gates to recover the canonical element order of the "discrete Fourier transform" ("DFT"), as many applications of the subroutine do not require this step (which is commonly included in other descriptions or definitions of the algorithm). To recover DFT amplitude ordering, further reverse the order of qubits in the result, before or else after the `qft`, with `swap` gates. (`swap` gates are commonly implemented via qubit label swap, hence they are virtually computationally "free," with no loss of generality in the case of a "fully-connected" qubit topologies, like in all QrackNet simulators.)


## Shor's algorithm (quantum period finding subroutine)

The quantum period finding subroutine of Shor's algorithm is practically "trivial" to express and simulate with QrackNet script:
```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [8], "output": "qsim" },

        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "h", "parameters": ["qsim", 1] },
        { "name": "h", "parameters": ["qsim", 2] },
        { "name": "h", "parameters": ["qsim", 3] },

        { "name": "pown", "parameters": ["qsim", [0, 1, 2, 3], [4, 5, 6, 7], 6, 15] },
        { "name": "iqft", "parameters": ["qsim", [0, 1, 2, 3]] },

        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2, 3], 16], "output": "result" }
    ]
}
```
We start the subroutine by preparing a maximal superposition of the input register. The (out-of-place operation) quantum "power modulo 'n'" base is chosen at random so it does not have a greater common divisor higher than 1 with the number to factor (`15`, in this example, and if a randomly chosen base _does_ have a greatest common divisor higher than 1 with the number to factor, then this condition effectively solves the factoring problem without need for the quantum subroutine at all). Then, the entire algorithm (to find the "period" of the "discrete logarithm") is "power modulo 'n'" out-of-place, from input register to intermediate calculation register, followed by just the "inverse quantum Fourier transform" on the input register (**without** terminal `swap` gates to reverse the order of qubits to match the classical "discrete Fourier transform), followed by measurement, until success. The output from this subroutine becomes a function input to another purely classical subroutine that uses this output to identify the input's factors with continued fraction representation, but see the [Wikipedia article on Shor's algorithm](https://en.wikipedia.org/wiki/Shor%27s_algorithm), for example, for a more detailed explanation.

These are output results we could receive from our quantum period finding subroutine job, immediately above:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "result": [
                0,
                0,
                5,
                0,
                0,
                0,
                0,
                6,
                0,
                0,
                2,
                14,
                10,
                0,
                9,
                0
            ]
        }
    }
}
```

## Schmidt decomposition rounding parameter (SDRP)

`set_sdrp` will reduce simulation fidelity (from "default ideal") to also reduce memory footprint and execution time. The "SDRP" setting becomes active in the program for the specific simulator at the point of calling `set_sdrp`. The value can be updated, through later `set_sdrp` calls in the circuit. It takes a value from `0.0` (no "rounding") to `1.0` ("round" away all entanglement):
```json
{
    "program" : [
        { "name": "init_general", "parameters": [2], "output": "qsim" },
        { "name": "set_sdrp", "parameters": ["qsim", 0.5] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "get_unitary_fidelity", "parameters": ["qsim"], "output": "fidelity" },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1], 8], "output": "result" }
    ]
}
```
This is an example of the job result:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "fidelity": 1,
            "result": [
                0,
                0,
                0,
                0,
                0,
                0,
                3,
                3
            ]
        }
    }
}
```
Note that, in this case, a relatively severe SDRP floating-point setting had no negative effect on the fidelity at all. The rounding effect would become apparent for a more complicated circuit, like a "quantum volume" or "random circuit sampling" case, for example.

## Near-Clifford rounding parameter (SDRP)

### Usage and syntax

`init_stabilizer` mode offers "near-Clifford" simulation, for Clifford gate set plus arbitrary (non-Clifford) single-qubit variational (and discrete) phase gates. (If gates outside of this set are applied, the stabilizer simulation will fall back to a universal method, and near-Clifford techniques will not apply.) This simulation method is "ideal" (not approximate), but it can be entirely prohibitively slow. To increase speed and reduce memory footprint at the cost of reduced fidelity, `set_ncrp` sets a "near-Clifford rounding parameter" that controls how small a non-Clifford phase effect gate can be (as a phase angle fraction of a `t` or `adjt` gate, whichever is positive) before it is "rounded" to no-operation instead of applied. "NCRP" comes into play at the point of measurement or expectation value output. It takes a value from `0.0` (no "rounding") to `1.0` ("round" away all non-Clifford behavior):
```json
{
    "program" : [
        { "name": "init_stabilizer", "parameters": [2], "output": "qsim" },
        { "name": "set_ncrp", "parameters": ["qsim", 1.0] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "t", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "measure_all", "parameters": ["qsim"], "output": "result" },
        { "name": "get_unitary_fidelity", "parameters": ["qsim"], "output": "fidelity" }
    ]
}
```
This is an example of the job result:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "result": 2,
            "fidelity": 0.6218592686818538
        }
    }
}
```

While "NCRP" values other than `1.0` and `0.0` are meaningful, it is highly suggested that `1.0` is used if this approximation technique is used at all. Applying even a single non-Clifford gate is simply too slow for practicality, otherwise.

### Conceptual "NCRP"

**(This section was drafted by QrackNet Quantum Guru, with help from the QrackNet developers!)** 

When working with near-Clifford simulations in QrackNet, it's important to keep in mind certain nuances that can impact the results and their interpretation. The near-Clifford rounding parameter (NCRP) plays a crucial role in managing the trade-off between fidelity and computational efficiency. Here are some key points to consider:

1. Omission of Phase Gates Before Measurement: In near-Clifford simulations, phase gates (such as T gates) immediately before measurement are effectively omitted, as they do not contribute to physically observable effects in the measurement outcomes. This means that even with user code containing phase gates, the fidelity of the state might still be reported as 1.0, reflecting the fact that these gates do not alter the measurement results.
2. User Code Gates Don't Correspond to Rounding Events: In fact, besides the case of phase gates immediately before measurement, the near-Clifford simulation technique in Qrack has a complicated and internally managed state, to greatly reduce the overall need to apply non-Clifford "magic". An `r` (Pauli-Z rotation) or `t` gate in user code simply will not correspond to a rounding event in place, most of the time.
3. Reporting Fidelity Estimates: When utilizing NCRP in your simulations, it's highly recommended to include fidelity estimates in your results. This is because knowing the fidelity is crucial for assessing the accuracy and reliability of your simulation, particularly in industrial and practical quantum computing scenarios. The fidelity estimate provides a quantitative measure of how closely the simulation approximates ideal quantum behavior, taking into account the effects of the NCRP setting.
4. Practical Example: To illustrate these points, consider a simple circuit with a series of Hadamard and CNOT gates, followed by T gates and a final measurement. Even though the T gates are present in the user code, their effect is not observable in the final measurement due to the nature of near-Clifford simulation. Therefore, it's important to report both the measurement outcomes and the fidelity estimate to fully understand the implications of the NCRP setting on your simulation.

By incorporating these considerations into your quantum circuit design and analysis, you can better interpret the results of near-Clifford simulations and make more informed decisions in your quantum computing projects.

## Random circuit sampling

"BQP-complete" complexity class is isomorphic to (maximally) random circuit sampling. It might be laborious to randomize the the coupler connections via (CNOT) `mcx` and to randomize the `u` double-precision angle parameters, but here is an example that is (unfortunately) not at all random in these choices, for 4 layers of a 4-qubit-wide unitary circuit of a "quantum volume" protocol (where `double` parameters should be uniformly random on [0,2π) or a gauge-symmetric interval, up to overall non-observable phase factor of degeneracy on [0,4π) which becames "physical" for controlled `mcu` operations):
```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [4], "output": "qsim" },
        { "name": "u", "parameters": ["qsim", 0, 0.1, 0.2, 0.3] },
        { "name": "u", "parameters": ["qsim", 1, 0.0, 3.14159265359, 6.28318531] },
        { "name": "u", "parameters": ["qsim", 2, 0.7, 4.2, 1.8] },
        { "name": "u", "parameters": ["qsim", 3, 1.0, 2.0, 3.0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "mcx", "parameters": ["qsim", [2], 3] },

        { "name": "u", "parameters": ["qsim", 0, 0.1, 0.2, 0.3] },
        { "name": "u", "parameters": ["qsim", 1, 0.0, 3.14159265359, 6.28318531] },
        { "name": "u", "parameters": ["qsim", 2, 0.7, 4.2, 1] },
        { "name": "u", "parameters": ["qsim", 3, 1.0, 2.0, 3.0] },
        { "name": "mcx", "parameters": ["qsim", [0], 2] },
        { "name": "mcx", "parameters": ["qsim", [1], 3] },

        { "name": "u", "parameters": ["qsim", 0, 0.1, 0.2, 0.3] },
        { "name": "u", "parameters": ["qsim", 1, 0.0, 3.14159265359, 6.28318531] },
        { "name": "u", "parameters": ["qsim", 2, 0.7, 4.2, 1] },
        { "name": "u", "parameters": ["qsim", 3, 1.0, 2.0, 3.0] },
        { "name": "mcx", "parameters": ["qsim", [0], 3] },
        { "name": "mcx", "parameters": ["qsim", [1], 2] },

        { "name": "u", "parameters": ["qsim", 0, 0.1, 0.2, 0.3] },
        { "name": "u", "parameters": ["qsim", 1, 0.0, 3.14159265359, 6.28318531] },
        { "name": "u", "parameters": ["qsim", 2, 0.7, 4.2, 1] },
        { "name": "u", "parameters": ["qsim", 3, 1.0, 2.0, 3.0] },
        { "name": "mcx", "parameters": ["qsim", [1], 2] },
        { "name": "mcx", "parameters": ["qsim", [0], 3] },

        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2, 3], 16], "output": "result" }
    ]
}
```
Conceptually, `init_general` would definitely be the right simulator optimization to use. For now, it is bugged even on this case. (As noted elsewhere, strongly prefer `init_qbdd` for general case.)

## (Optional advanced usage:) quantum neurons

This is an arbitrary training for a "quantum neuron":
```json
{
    "program" : [
        { "name": "init_qbdd", "parameters": [2], "output": "qsim" },
        { "name": "init_qneuron", "parameters": ["qsim", [0], 1, 0, 0, 1e-6], "output": "qneuron" },
        { "name": "write_bool", "parameters": [false], "output": "fLit" },
        { "name": "write_bool", "parameters": [true], "output": "tLit" },
        { "name": "qneuron_learn", "parameters": ["qneuron", 0.25, "fLit", true] },
        { "name": "x", "parameters": ["qsim", 0] },
        { "name": "qneuron_learn", "parameters": ["qneuron", 0.5, "tLit", true] },
        { "name": "x", "parameters": ["qsim", 0] },

        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "qneuron_predict", "parameters": ["qneuron", "tLit", false], "output": "n0" },

        { "name": "measure_shots", "parameters": ["qsim", [0, 1], 8], "output": "result" },
        { "name": "get_qneuron_angles", "parameters": ["qneuron"], "output": "neuronParams" }
    ]
}
```
Qubits involved in neuron learning and prediction act like "synapses." Booleans for quantum neuron subset API can be specified as literals or as `output` space variable names (as immediately above). In `qneuron_learn` and `qneuron_predict` method signatures, the first occurring boolean argument controls whether the intended (single) "target" qubit state is considered `true` or `false`. The second occurring boolean argument controls whether the output qubit is set to |+> state in X-basis (`h` gate on |0>, from Z-basis) before carrying out learning or prediction.

This is a real example of the output:
```json
{
    "message": "Retrieved job status and output by ID.",
    "data": {
        "status": {
            "id": 1,
            "name": "SUCCESS",
            "message": "Job completed fully and normally."
        },
        "output": {
            "qsim": 0,
            "qneuron": "0",
            "fLit": false,
            "tLit": true,
            "n0": 0.926776647567749,
            "result": [
                0,
                2,
                2,
                2,
                3,
                3,
                3,
                3
            ],
            "neuronParams": [
                0.7853981852531433,
                1.5707963705062866
            ]
        }
    }
}
```
The `neuronParams` count of array elements would scale like 2 to the power in input "`c`" ("control") argument qubits, while each neuron has a single target target qubit, in the requested qubit simulator.

**Powered by Qrack! You rock!**

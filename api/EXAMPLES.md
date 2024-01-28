# QrackNet Usage Examples

QrackNet may be deployed with or without authentication enabled. We assume that you have authenicated as appropriate to the particular deployment and hold a session cookie (if necessary).

## "Flipping a (Simulated) Quantum Coin"

Say that we want to measure a maximal superposition of single qubit for a random outcome, like "flipping a coin." The QrackNet API script is as simple as follows.

At the `POST /api/qrack` route, we give the following request body:
```json
{
    "program" : [
        { "name": "init_general", "parameters": [1], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "measure", "parameters": ["qsim", 0], "output": "result" }
    ]
}
```
Notice that the first operation in the list creates a simulator (with one qubit) and saves its ID as variable name "`qsim`" in the job's "`output`" space. When we need to supply a simulator ID to further operations (which we do for nearly all operations in the scripting API), notice that we specify the simulator ID as the name of this "`qsim`" variable, which resolves the output value of simulator initialization operation.

Qubits are numbered from 0, by default: the next operation acts a Hadamard ("`h`") gate on the only qubit, then "`measure`" measures it. Because "`measure`" specifies that its result should be saved to the "global job output space" as "`result`," we expect to read our coin flip result from this same variable name in the "`output`" object field of the job status, where `output` schema is defined entirely by the user program.

We receive a response like the following, to the `POST` request:
```json
{
    "message": "Created new job for user.",
    "data": {
        "id": 1,
        "userId": 1,
        "jobStatusTypeId": 3,
        "statusMessage": "RUNNING",
        "updatedAt": "2024-01-22T12:53:40.402Z",
        "createdAt": "2024-01-22T12:53:40.402Z"
    }
}
```
We see from the "`message`" field, the job was created successfully. However, our syntax wasn't fully validated when the server simply accepted the request, and the job might still fail. A short time later, we can check the job status and output values, using the "`id`" field for this job under "`data`".

At the `GET /api/qrack/1` route, to retrieve our job ID "1," we get the following job status and output, where user-defined `"result"` property of `output` is expected to be either `true` or `false`:
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
            "result": true
        }
    }
}
```

Under the "`data`" field, we see that the status is "SUCCESS" (code ID "1"). Then, if we look at the "`output`" field under `data`, this contains our entire "output space" for the job, where all entries were originally defined by the `output` fields of the script we submitted. `"qsim"` is the simulator ID we used, which "lives" in the output space, but it's not of particular use to us, except when we referenced it by variable name in the original script request. "`result`" under `output` is the name we gave to the output of the `measure` operation: our "coin flip" result is "`true`" (vs. "`false`")!

## Flipping Two Entangled "Coins"

Say that we want to expand the first example to showcase not just superposition, but also entanglement. We can repeat the experiment with a "[Bell pair](https://en.wikipedia.org/wiki/Bell_state)."

At the `POST /api/qrack` route, we give the following request body:
```json
{
    "program" : [
        { "name": "init_general", "parameters": [2], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1], 8], "output": "result" }
    ]
}
```
This time, we create a simulator with 2 qubits, rather than 1. We prepare the state similarly to before, with `h` gate. However, we also act a CNOT gate ("`mcx`," "multiply-controlled Pauli X") with one control qubit, as `[0]` array, where the gate can accept as many control qubits as we want in that array, and the single target qubit ID "`1`." These two gates, on a reset or freshly-initialized register, produce a Bell state. To demonstrate entanglement, we measure the qubits with `measure_shots`, for both qubits (`[0, 1]`), for `8` repetitions of the circuit.

We receive a response like the following, at the `GET /api/qrack/2` route, to retrieve our job ID "1,":
```json
{
    "message": "Created new job for user.",
    "data": {
        "id": 2,
        "userId": 1,
        "jobStatusTypeId": 3,
        "statusMessage": "RUNNING",
        "updatedAt": "2024-01-22T13:14:22.218Z",
        "createdAt": "2024-01-22T13:14:22.218Z"
    }
}
```

Sometime later, we check on the job status and output:
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
            "qsim": 1,
            "result": [
                0,
                0,
                0,
                0,
                0,
                3,
                3,
                3
            ]
        }
    }
}
```

Our job was successful, again (though `"result"` field values will differ, experiment to experiment). Our output "`result`" field (called so because we specified that name as "`output`" parameter to `measure_shots`) contains 8 "shots" of the circuit, as a terminal measurement result. Notice that `0` and `3` appear in the binary (integer) representation of our two qubits, but `1` and `2` do not. This is because, according to how we prepared our state, both qubits must be in the same state upon measurement, either both |0> or both |1>. (We have completed another successful example!)

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
        { "name": "mcz", "parameters": ["qsim", [0], 2] },
        { "name": "mcx", "parameters": ["qsim", [1], 2] },
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

## 30-qubit GHZ state

The QrackNet server is likely to successfully run even 50-qubit or larger GHZ state preparation circuits (with measurement), though reporting precision for results might currently be limited to about 32 bits. This definitely won't work with `init_general` (which will ultimately attempt to allocate 30+ qubits of state vector representation) but it's likely to work with `init_stabilizer` and `init_qbdd`, as both are optimized (differently) for this GHZ-state case.

We can think of the state preparation as an `h` gate followed by a loop over `mcx` gates with single control qubits, but we need to "manually unroll" the conceptual loop:
```json
{
    "program" : [
        { "name": "init_stabilizer", "parameters": [30], "output": "qsim" },
        { "name": "h", "parameters": ["qsim", 0] },
        { "name": "mcx", "parameters": ["qsim", [0], 1] },
        { "name": "mcx", "parameters": ["qsim", [0], 2] },
        { "name": "mcx", "parameters": ["qsim", [0], 3] },
        { "name": "mcx", "parameters": ["qsim", [0], 4] },
        { "name": "mcx", "parameters": ["qsim", [0], 5] },
        { "name": "mcx", "parameters": ["qsim", [0], 6] },
        { "name": "mcx", "parameters": ["qsim", [0], 7] },
        { "name": "mcx", "parameters": ["qsim", [0], 8] },
        { "name": "mcx", "parameters": ["qsim", [0], 9] },
        { "name": "mcx", "parameters": ["qsim", [0], 10] },
        { "name": "mcx", "parameters": ["qsim", [0], 11] },
        { "name": "mcx", "parameters": ["qsim", [0], 12] },
        { "name": "mcx", "parameters": ["qsim", [0], 13] },
        { "name": "mcx", "parameters": ["qsim", [0], 14] },
        { "name": "mcx", "parameters": ["qsim", [0], 15] },
        { "name": "mcx", "parameters": ["qsim", [0], 16] },
        { "name": "mcx", "parameters": ["qsim", [0], 17] },
        { "name": "mcx", "parameters": ["qsim", [0], 18] },
        { "name": "mcx", "parameters": ["qsim", [0], 19] },
        { "name": "mcx", "parameters": ["qsim", [0], 20] },
        { "name": "mcx", "parameters": ["qsim", [0], 21] },
        { "name": "mcx", "parameters": ["qsim", [0], 22] },
        { "name": "mcx", "parameters": ["qsim", [0], 23] },
        { "name": "mcx", "parameters": ["qsim", [0], 24] },
        { "name": "mcx", "parameters": ["qsim", [0], 25] },
        { "name": "mcx", "parameters": ["qsim", [0], 26] },
        { "name": "mcx", "parameters": ["qsim", [0], 27] },
        { "name": "mcx", "parameters": ["qsim", [0], 28] },
        { "name": "mcx", "parameters": ["qsim", [0], 29] },
        { "name": "measure_shots", "parameters": ["qsim", [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29], 100], "output": "result" }
    ]
}
```
If we exceed Clifford gate set in any single line of the script, we are likely (but not guaranteed) to fall back to state vector with `init_stabilizer`, leading to a (self-recovering, handled) "server crash." On the other hand, `init_qbdd` can also efficiently handle this state preparation without the requirement to constrain one's gate set "vocabularly" to pure Clifford gates. (This might make QBDD the more powerful option in many cases where GHZ-state preparation is only one "subroutine" in a larger quantum algorithm with a universal gate set.)

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
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823,
                1073741823
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
`iqft` instruction is similarly the "inverse QFT", with the same syntax. The method does **not** apply terminal `swap` gates to recover the canonical element order of the "discrete Fourier transform" ("DFT"), as many applications of the subroutine do not require this step (which is commonly included in descriptions of the algorithm). To recover DFT amplitude ordering, further reverse the order of qubit in the result, after the `qft`, with `swap` gates. (`swap` gates are commonly implemented via qubit label swap, hence they are virtually computationally "free," with no loss of generality in the case of a "fully-connected" qubit topologies, like in all QrackNet simulators.)


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

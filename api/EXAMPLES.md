# QrackNet Usage Examples

QrackNet may be deployed with or without authentication enabled. We assume that you have authenicated as appropriate to the particular deployment and hold a session cookie (if necessary).

## "Flipping a (Simulated) Quantum Coin"

Say that we want to measure a maximal superposition of single qubit for a random outcome, like "flipping a coin." The QrackNet API script is as simple as follows.

At the `POST /api/qrack` route, we give the following request body:
```
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

We receive a response like the following:
```
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

At the `GET /api/qrack/1` route, to retrieve our job ID "1," we get the following job status and output:
```
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

Under the "`data`" field, we see that the status is "SUCCESS" (code ID "1"). Then, if we look at the "`output`" field under `data`, this contains our entire "output space" for the job, where all entries were originally defined by the `output` fields of the script we submitted. `qsim` is the simulator ID we used, which "lives" in the output space, but it's not of particular use to us, except when we referenced it by variable name in the original script request. "`result`" under `output` is the name we gave to the output of the `measure` operation: our "coin flip" result is "`true`" (vs. "`false`")!

## Flipping Two Entangled "Coins"

Say that we want to expand the first example to showcase not just superposition, but also entanglement. We can repeat the experiment with a "[Bell pair](https://en.wikipedia.org/wiki/Bell_state)."

At the `POST /api/qrack` route, we give the following request body:
```
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

We receive a response like the following:
```
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
```
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

Our job was successful, again. Our output "`result`" field (called so because we specified that name as "`output`" parameter to `measure_shots`) contains 8 "shots" of the circuit, as a terminal measurement result. Notice that `0` and `3` appear in the binary (integer) representation of our two qubits, but `1` and `2` do not. This is because, according to how we prepared our state, both qubits must be in the same state upon measurement, either both |0> or both |1>. (We have completed another successful example!)

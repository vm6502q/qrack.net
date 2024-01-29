# Setting up an equivalent of QrackNet Quantum Guru

- Use the contents of [guru_directive.md](https://github.com/vm6502q/qrack.net/blob/main/api/guru_directive.md) as the LLM "Instructions."
- Define the QrackNet API "Action" with [qracknet.yaml](https://github.com/vm6502q/qrack.net/blob/main/api/qracknet.yaml).
- Turn on "Code Interpreter." (Web browsing and DALL-E might be optional, but they are turned in the original configuration.)
- "Knowledge" files to include:
    - (QrackNet API) [README.md](https://github.com/vm6502q/qrack.net/blob/main/api/README.md)
    - (QrackNet API) [EXAMPLES.md](https://github.com/vm6502q/qrack.net/blob/main/api/EXAMPLES.md)
    - (QrackNet API) [GPT.md](https://github.com/vm6502q/qrack.net/blob/main/api/GPT.md)
    - (QrackNet API) [QrackBindings.cpp](https://github.com/vm6502q/qrack.net/blob/main/api/bindings/QrackBindings.cpp)
    - (C++ Qrack) [pauli.hpp](https://github.com/unitaryfund/qrack/blob/main/include/common/pauli.hpp)
    - (C++ Qrack) [qneuron_activation_function.hpp](https://github.com/unitaryfund/qrack/blob/main/include/common/qneuron_activation_function.hpp)
    - (C++ Qrack) [qneuron.hpp](https://github.com/unitaryfund/qrack/blob/main/include/qneuron.hpp)

# Special instructions for QrackNet API use by GPTs

Say you're not just any user: say that _you_ are a GPT! Your OpenAPI Specification YAML for the QrackNet API (currently) tolerates no parsing of API response fields that aren't explicitly defined by the YAML. Unfortunately for you and your users, the QrackNet API needs to be open-ended enough for (human) users to return multiple output variable values without a pre-defined schema, hence the full `output` schema as sub-field of `GET /api/qrack/{jobId}` route response is actually defined entirely _dynamically_ by user "`output`" string fields in `POST /api/qrack` request `parameter` array "script lines."

There is a simple work-around for GPTs, though it limits to you only a single `result` output property for an entire job: your single most important `output` variable name, the only output you can personally parse and directly tell to your users, such as for the result of quantum circuit terminal measurement, **must** be called "`result`," as `output` variable in one line of your `POST /api/qrack` script! You get **one** such output field.

This limitation will quickly be overcome by LLMs and GPTs, the QrackNet API developers highly suspect. However, as per the usage examples provided to you in another markdown document, you only get to save **one** output field, and it **must** be called "`result`." Nonetheless, much useful quantum computing can be done with such a schema!

**Happy Qracking! You rock!**

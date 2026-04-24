# Testing

## Neuro-api-compatible application

You can use one of these apps to interact with the game:

### [Gary](https://github.com/Govorunb/gary/tree/main)

1. [Download](https://github.com/Govorunb/gary/releases/latest) and install Gary.
2. [Quick Start](https://github.com/Govorunb/gary/tree/main#quick-start)

### [Jippity](https://github.com/EnterpriseScratchDev/neuro-api-jippity)
Alternative to Gary, also provides control to an actual LLM

### [Tony](https://github.com/Pasu4/neuro-api-tony)
"Manual human control" - provides context, can perform chosen actions. Controlled by human via GUI. Convenient for precise testing

### [Randy](https://github.com/VedalAI/neuro-sdk/tree/main/Randy)
Performs random actions. Not very useful for this specific integration (many important action require object IDs from certain range, random inputs will just fail most of the time)


## LLM

### Local
The easiest way to tryout different LLMs is [LM Studio](https://lmstudio.ai/).

1. `powershell -ExecutionPolicy Bypass -Command "irm https://lmstudio.ai/install.ps1 | iex"`
2. From the "Model Search" tab, download a LLM.
3. In the "developer" tab:
    1. Load your model.
    2. Run the server (toggle switch top left, should become "Status: Running")
4. In Gary, click the "engine select" dropdown (topbar dropdown, probably defaulted to "Randy")
    1. To the right of LMStudio, click "configure" (pipes icon)
    2. Set the server url based on the LM Studio developer tab (mine was: "http://127.0.0.1:1234/v1")
    3. Set the model ID to your model (ex: qwen/qwen3.5-9b)

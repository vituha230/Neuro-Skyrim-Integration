# Testing

## System Overview
The full chain allowing a Large Language Model (LLM) to play Skyrim looks like this:
```
[Skyrim]+[SKSE]+[Neuro Skyrim Integration]+[Neuro SDK] <---> [Gary] <---> [LLM]
```
Skyrim has game state (player position, status, surroundings, etc.) and a bunch of possible interactions (bartering, enchanting, walking, etc.). Obviously, by itself, it can't delegate interpreting state and deciding actions to an LLM.

The first step is accessing Skyrim's state and controlling the player, via the Skyrim Script Extender (SKSE).

The Neuro Skyrim Integration leverages SKSE to analyze Skyrim's game state and generate a text context to pass to the LLM. Then, it provides a text description of actions the LLM may take. Finally, via SKSE, it performs the action chosen in Skyrim.

Neuro Software Development Kit (SDK) allows the integration to communicate to an LLM over network (<-->).

Gary serves as a test harness for LLMs. It implements the Neuro SDK interface and communicates over network between the integration and an open-ai compatible LLM.

Finally, an open-ai compatible LLM determines an action from the provided context + list of actions and sends its choice back to Gary.

## [Gary](https://github.com/Govorunb/gary/tree/main)

1. [Download](https://github.com/Govorunb/gary/releases/latest) and install Gary.
2. [Quick Start](https://github.com/Govorunb/gary/tree/main#quick-start)

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

### Remote
Lookup documentation from your provider and configure Gary to connect. Good luck!

## Gary Cont'd

1. Online the server (top left button)
2. Add a connection (diagnostics example)
3. Verify Gary is connected to LLM
4. Launch Skyrim with Neuro Integration
5. Enable "Auto-act"
6. Watch AI Slay Dragons!!! Welp probably not... it's kinda derpy. So start tweaking:
    * Try different LLMs
    * Edit the mod
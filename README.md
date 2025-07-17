<h1 align="center">MPMDManager</h1>

<p align="center">
  An abstraction layer built on top of MPI that simplifies development of parallel programs in MPMD (Multiple Program Multiple Data) environments by providing intuitive functions for program identification, communication, and coordination.
</p>

![Preview](https://github.com/user-attachments/assets/e7255479-e9a6-4478-ba8f-ad05f6373820)
![License](https://img.shields.io/github/license/RiadOliveira/MPMDManager)

Contents
=================
<!--ts-->
* [💡 Proposal](#proposal)
* [🛠️ Technologies](#technologies)
* [🚀 Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
  * [Project Structure](#structure)
  * [Running Examples](#running-examples)
* [⚙️ Features](#features)
  * [Single World](#features-single)
  * [Multi World (Client)](#features-multi-client)
  * [Multi World (Server)](#features-multi-server)
* [📚 API Reference](#reference)
  * [Single World](#reference-single)
  * [Multi World (Client)](#reference-multi-client)
  * [Multi World (Server)](#reference-multi-server)
* [📜 Certification](#certification)
* [📝 License](#license)
* [👨‍💻 Authors](#authors)
<!--te-->
<br/>

<h2 id="proposal">💡 Proposal</h2>
MPMDManager provides two powerful abstractions for parallel programming:

- **Single World**: Designed for programs launched together in MPI's MPMD mode (using colon-separated commands) where all processes share the same `MPI_COMM_WORLD`.
  
- **Multi World**: For independently launched programs, each with their own `MPI_COMM_WORLD`, using MPI's client/server model with an `ompi-server` intermediary.

Available in both **C** (using native MPI) and **Python** (via mpi4py), these abstractions maintain identical functionality across languages while accommodating different developer preferences and use cases. <br/><br/>

<h2 id="technologies">🛠️ Technologies</h2>
Built with:

* [C](https://en.cppreference.com/w/c/language.html)
  * [MPI (OpenMPI)](https://www.open-mpi.org/)
* [Python](https://www.python.org/)
  * [mpi4py](https://mpi4py.readthedocs.io/en/stable/) <br/><br/>

<h2 id="getting-started">🚀 Getting Started</h2>

<h3 id="prerequisites">Prerequisites</h3>

- MPI implementation (OpenMPI recommended)
- C compiler (GCC recommended)
- Python and mpi4py
- Git

<h3 id="installation">Installation</h3>

```bash
# Clone the repository
$ git clone https://github.com/RiadOliveira/MPMDManager.git

# Navigate to project directory
$ cd MPMDManager
```

<h3 id="structure">Project Structure</h3>
The repository is organized as follows:

```text
src/
├── single_world/
│   ├── c/
│   │   ├── abstraction/    # C implementation of Single World
│   │   ├── example/        # Example usage in C
│   │   └── run_example.sh  # Compiles and runs C example
│   └── python/
│       ├── abstraction/    # Python implementation
│       ├── example/        # Python example
│       └── run_example.sh  # Runs Python example
└── multi_world/
    ├── c/
    │   ├── abstraction/    # C implementation of Multi World
    │   ├── example/        # Client and server example
    │   └── run_example.sh  # Compiles and runs examples
    └── python/
        ├── abstraction/    # Python implementation
        ├── example/        # Python example
        └── run_example.sh  # Runs Python examples
```

<h3 id="running-examples">Running Examples</h3>
To execute any example, navigate to the corresponding implementation directory:

```bash
cd src/<abstraction>/<language>/
./run_example.sh
```

Where:
* ```<abstraction>``` is either ```single_world``` or ```multi_world```.
* ```<language>``` is either ```c``` or ```python```.

Each ```run_example.sh``` script handles all necessary setup including:
* Automatic compilation (for C implementations).
* MPI environment configuration.
* Clean execution.
* Proper cleanup of temporary files.

<h2 id="features">⚙️ Features</h2>

<h3 id="features-single">Single World</h3>

* Retrieve local program information (name, communicator, size).
* Access process-specific details (rank).
* Obtain remote program data by name or index (communicator, size).
* Simplified inter-program communication setup.

<h3 id="features-multi-client">Multi World (Client)</h3>

* Connect to named servers with customizable retry logic by specifying:
  * Max Attempts.
  * Initial Wait Time (in milliseconds).
  * Wait Increment Time (in milliseconds, added to the wait time for each attempt).
  * Max Wait Time (in milliseconds).
* Manage multiple server connections simultaneously.
* Retrieve server communicators by name or index.
* Disconnection from individual or all servers.

<h3 id="features-multi-server">Multi World (Server)</h3>

* Server lifecycle management (open/close).
* Client connection approval with optional naming.
* Client communicator retrieval by name or index.
* Selective or complete client disconnection.
* Server naming and identification.

<br/>

<h2 id="reference">📚 API Reference</h2>

<h3 id="reference-notes">Notes</h3>

* All functions terminate with <code>MPI_Abort()</code> on failure.
* Connection IDs can be names (strings) or indices (integers).
* Python implementations use static methods for direct class access.

<h3 id="reference-single">Single World</h3>

<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Description</th>
      <th>C Syntax</th>
      <th>Python Syntax</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><strong>Init</strong></td>
      <td>Initializes manager with program arguments</td>
      <td><code>const MPMDManager* Manager_Init(char** argv)</code></td>
      <td><code>MPMDManager.init() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Finalize</strong></td>
      <td>Cleans up manager resources</td>
      <td><code>void Manager_Finalize(const MPMDManager*)</code></td>
      <td><code>MPMDManager.finalize() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Local Name</strong></td>
      <td>Gets local program name</td>
      <td><code>const char* Manager_Local_name()</code></td>
      <td><code>MPMDManager.local_name() -> str</code></td>
    </tr>
    <tr>
      <td><strong>Local Comm</strong></td>
      <td>Gets local communicator</td>
      <td><code>const MPI_Comm* Manager_Local_comm()</code></td>
      <td><code>MPMDManager.local_comm() -> MPI.Intracomm</code></td>
    </tr>
    <tr>
      <td><strong>Local Rank</strong></td>
      <td>Gets process rank</td>
      <td><code>int Manager_Local_rank()</code></td>
      <td><code>MPMDManager.local_rank() -> int</code></td>
    </tr>
    <tr>
      <td><strong>Local Size</strong></td>
      <td>Gets program size</td>
      <td><code>unsigned int Manager_Local_size()</code></td>
      <td><code>MPMDManager.local_size() -> int</code></td>
    </tr>
    <tr>
      <td><strong>Comm To</strong></td>
      <td>Gets remote communicator</td>
      <td><code>const MPI_Comm* Manager_Comm_to(ConnectionId, IdType)</code></td>
      <td><code>MPMDManager.comm_to(identifier: str | int) -> MPI.Comm</code></td>
    </tr>
    <tr>
      <td><strong>Size Of</strong></td>
      <td>Gets remote program size</td>
      <td><code>unsigned int Manager_Size_of(ConnectionId, IdType)</code></td>
      <td><code>MPMDManager.size_of(identifier: str | int) -> int</code></td>
    </tr>
  </tbody>
</table>

<h3 id="reference-multi-client">Multi World (Client)</h3>

<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Description</th>
      <th>C Syntax</th>
      <th>Python Syntax</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><strong>Init</strong></td>
      <td>Initializes client manager</td>
      <td><code>const ClientManager* Client_Init(uint maxServers)</code></td>
      <td><code>ClientManager.init() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Finalize</strong></td>
      <td>Cleans up client resources</td>
      <td><code>void Client_Finalize(const ClientManager*)</code></td>
      <td><code>ClientManager.finalize() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Connect</strong></td>
      <td>Connects to named server with retry logic</td>
      <td><code>const MPI_Comm* Client_Connect(const char*, const ConnectAttemptData*)</code></td>
      <td><code>ClientManager.connect(server_name: str, attemptData=ConnectAttemptData()) -> MPI.Intercomm</code></td>
    </tr>
    <tr>
      <td><strong>Retrieve Server Comm</strong></td>
      <td>Gets server communicator</td>
      <td><code>const MPI_Comm* Client_Retrieve_Server_comm(ConnectionId, IdType)</code></td>
      <td><code>ClientManager.retrieve_server_comm(server_id: str | int) -> MPI.Intercomm</code></td>
    </tr>
    <tr>
      <td><strong>Disconnect Server</strong></td>
      <td>Disconnects from specific server</td>
      <td><code>void Client_Disconnect_server(ConnectionId, IdType)</code></td>
      <td><code>ClientManager.disconnect_server(server_id: str | int) -> None</code></td>
    </tr>
    <tr>
      <td><strong>Disconnect Servers</strong></td>
      <td>Disconnects from all servers</td>
      <td><code>void Client_Disconnect_servers()</code></td>
      <td><code>ClientManager.disconnect_servers() -> None</code></td>
    </tr>
  </tbody>
</table>

<h3 id="reference-multi-server">Multi World (Server)</h3>

<table>
  <thead>
    <tr>
      <th>Function</th>
      <th>Description</th>
      <th>C Syntax</th>
      <th>Python Syntax</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td><strong>Init</strong></td>
      <td>Initializes server manager</td>
      <td><code>const ServerManager* Server_Init(char**, const char*, uint)</code></td>
      <td><code>ServerManager.init(server_name: str | None = None) -> None</code></td>
    </tr>
    <tr>
      <td><strong>Finalize</strong></td>
      <td>Cleans up server resources</td>
      <td><code>void Server_Finalize(const ServerManager*)</code></td>
      <td><code>ServerManager.finalize() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Local Name</strong></td>
      <td>Gets server name</td>
      <td><code>const char* Server_Local_name()</code></td>
      <td><code>ServerManager.local_name() -> str</code></td>
    </tr>
    <tr>
      <td><strong>Open</strong></td>
      <td>Opens server for connections</td>
      <td><code>void Server_Open()</code></td>
      <td><code>ServerManager.open() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Close</strong></td>
      <td>Closes server</td>
      <td><code>void Server_Close()</code></td>
      <td><code>ServerManager.close() -> None</code></td>
    </tr>
    <tr>
      <td><strong>Accept</strong></td>
      <td>Accepts client connection</td>
      <td><code>const MPI_Comm* Server_Accept(const char*)</code></td>
      <td><code>ServerManager.accept(client_id: str | None = None) -> Intercomm</code></td>
    </tr>
    <tr>
      <td><strong>Retrieve Client Comm</strong></td>
      <td>Gets client communicator</td>
      <td><code>const MPI_Comm* Server_Retrieve_Client_comm(ConnectionId, IdType)</code></td>
      <td><code>ServerManager.retrieve_client_comm(client_id: str | int) -> MPI.Intercomm</code></td>
    </tr>
    <tr>
      <td><strong>Disconnect Client</strong></td>
      <td>Disconnects specific client</td>
      <td><code>void Server_Disconnect_client(ConnectionId, IdType)</code></td>
      <td><code>ServerManager.disconnect_client(client_id: str | int) -> None</code></td>
    </tr>
    <tr>
      <td><strong>Disconnect Clients</strong></td>
      <td>Disconnects all clients</td>
      <td><code>void Server_Disconnect_clients()</code></td>
      <td><code>ServerManager.disconnect_clients() -> None</code></td>
    </tr>
  </tbody>
</table>

<h2 id="certification">📜 Certification</h2>

This software is officially registered with the **Brazilian National Institute of Industrial Property (INPI)** as a Computer Program:

- **Certificate Type**: Certificate of Registration of a Computer Program
- **Registration Number**: BR512025000477-0
- **Issuance Date**: February 2025
- **Expiration Date**: January 2076
- **Certificate URL**: [MPMDManager Certificate](https://drive.google.com/file/d/1yp7-32CHxvs7p2AMxBkyF8EtoYUkXeRQ/view?usp=sharing)
- **Issuing Authority**: [INPI Brazil](https://www.gov.br/inpi/en)

<h2 id="license">📝 License</h2>
This project is MIT Licensed. See <a href="https://github.com/RiadOliveira/MPMDManager/blob/main/LICENSE">LICENSE</a> file for more details.

<h2 id="authors">👨‍💻 Authors</h2>

<kbd>
 <a href="https://github.com/RiadOliveira">
   <img src="https://avatars.githubusercontent.com/u/69125013?v=4" width="100" alt="Ríad Oliveira"/>
   <br/><br/>
   <p align="center"><b>Ríad Oliveira</b></p>
 </a>
</kbd>
<kbd>
 <a href="https://github.com/paulohenriquels">
   <img src="https://avatars.githubusercontent.com/u/102553073?v=4" width="100" alt="Paulo Henrique"/>
   <br/><br/>
   <p align="center"><b>Paulo Henrique</b></p>
 </a>
</kbd>

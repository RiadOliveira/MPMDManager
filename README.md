<h1 align="center">MPMDManager</h1>

<p align="center">
  An abstraction, built on top of the MPI library, that is designed to simplify the development of parallel programs in the MPMD context by providing functions to facilitate the identification, communication, and coordination of multiple parallel programs.
</p>

Contents
=================
   - [💡 Proposal](#proposal)
   - [🛠️ Technologies](#technologies)
     - [Languages](#languages)
     - [Libraries](#libraries)
   - [:gear: Features](#features)
     - [Single World](#features-single)
     - [Multi World (Client)](#features-multi-client)
     - [Multi World (Server)](#features-multi-server)
   - [:blue_book: API Reference](#reference)
     - [Single World](#reference-single)
     - [Multi World (Client)](#reference-multi-client)
     - [Multi World (Server)](#reference-multi-server)
   - [:memo: License](#license)
   - [👨‍💻 Authors](#authors)
</br>

<h2 id="proposal">💡 Proposal</h2>
<p>
  MPMDManager, as the name suggests, is a software tool that provides a manager for MPMD environments, simplifying the identification and connection procedures between parallel programs. The project introduces two distinct abstractions:
</p>

- **Single World**: Designed for cases where parallel programs are executed in MPI's MPMD mode, meaning all programs are launched with a single command line, separated by colons (:). In this context, all program processes share the same *MPI_COMM_WORLD*, which is why this abstraction is called *Single World*.
- **Multi World**: Intended for cases where parallel programs are executed independently, each with its own command line. In this scenario, each program has its own *MPI_COMM_WORLD* that encompasses only its own processes, hence the name *Multi World*. Communication in this abstraction is achieved using MPI's client/server methods, which require the execution of an intermediary server via the *ompi-server* command.

<p>
The abstractions are available in C, using the original MPI library, and in Python, through the <i>mpi4py</i> package. Both versions retain the functionality and capabilities of the <i>Single World</i> and <i>Multi World</i> abstractions, offering suitable alternatives for different needs and levels of expertise.
</p>

<h2 id="technologies">🛠️ Technologies</h2>

- <h3 id="languages">Languages</h3>

    &nbsp;![Technologies](https://skillicons.dev/icons?i=c,python&theme=dark)

- <h3 id="libraries">Libraries</h3>

  - ![MPI (OpenMPI)](https://www.open-mpi.org/)
  - ![mpi4py](https://mpi4py.readthedocs.io/en/stable/)

<h2 id="features">:gear: Features</h2>

<h3 id="features-single">Single World</h3>

- Local program information retrieval:
  - Program Name
  - Communicator
  - Size
- Local process information retrieval:
  - Rank
- Remote program information retrieval using its name or index:
  - Communicator
  - Size

<h3 id="features-multi-client">Multi World (Client)</h3>

- Request connection to a server using its name
- Customize connection attempts by specifying:
  - Max Attempts
  - Initial Wait Time (in milliseconds)
  - Wait Increment Time (in milliseconds, added to the wait time for each attempt)
  - Max Wait Time (in milliseconds)
- Retrieve communicators of previously connected servers by their names or indexes.
- Disconnect from a specific server by its name or index.
- Disconnect from all previously connected servers.

<h3 id="features-multi-server">Multi World (Server)</h3>

- Open and close the server.
- Approve client connection, with the option to assign a name to the client.
- Retrieve communicators of previously connected clients by their names or indexes.
- Disconnect a specific client by its name or index.
- Disconnect all previously connected clients.
- Retrieve the local server name.

<h2 id="reference">:blue_book: API Reference</h2>

<p>
This section provides a comprehensive overview of all functions offered by the abstractions, detailing their names, syntax, and descriptions. The documentation includes both <i>Single World</i> and <i>Multi World</i> abstractions, with the latter featuring a clear distinction between Client and Server functionalities. All functions are documented for both supported languages, C and Python, ensuring compatibility and clarity for developers across diverse platforms.

In C, the functionalities are implemented as standalone functions with specific naming conventions for clarity. For <i>Single World</i>, all functions are prefixed with ```Manager_```. For <i>Multi World</i>, functions are categorized into Client and Server, using the prefixes ```Client_``` and ```Server_```, respectively.

In Python, the approach differs slightly by using distinct classes to encapsulate the functionality. The ```MPMDManager``` class represents the <i>Single World</i> abstraction, while the <i>Multi World</i> abstraction is divided into two separate classes: ```ClientManager``` and ```ServerManager```. Each class employs static methods defined with the ```@staticmethod``` decorator, allowing the functions to be called directly from the class without requiring object instantiation.
</p>

<h3 id="reference-single">Single World</h3>

- **Init**:
  - Description: Initializes the manager by setting its communicator and connections. This routine performs communication among all processes, so it must be called by all of them at the start of their execution, immediately after ```MPI_Init()```.
  - Syntax:
    - C: ```const MPMDManager* Manager_Init(char** argv)```
    - Python: ```MPMDManager.init() -> None```
  - Note: In C, the function returns a pointer to the ```Manager```, which is required by other functions.
- **Finalize**:
  - Description: Finalizes the manager, freeing all allocated resources and terminating established connections.
  - Syntax:
    - C: ```void Manager_Finalize(const MPMDManager* manager)```
    - Python: ```MPMDManager.finalize() -> None```
- **Local Name**:
  - Description: Returns the name of the local program.
  - Syntax:
    - C: ```const char* Manager_Local_name(const MPMDManager* manager)```
    - Python: ```MPMDManager.local_name() -> str```
- **Local Comm**:
  - Description: Returns the communicator of the local program.
  - Syntax:
    - C: ```const MPI_Comm* Manager_Local_comm(const MPMDManager* manager)```
    - Python: ```MPMDManager.local_comm() -> MPI.Intracomm```
- **Local Rank**:
  - Description: Returns the rank of the process within the context of the local program.
  - Syntax:
    - C: ```int Manager_Local_rank(const MPMDManager* manager)```
    - Python: ```MPMDManager.local_rank() -> int```
- **Local Size**:
  - Description: Returns the size of the local program.
  - Syntax:
    - C: ```unsigned int Manager_Local_size(const MPMDManager* manager)```
    - Python: ```MPMDManager.local_size() -> int```
- **Comm To**:
  - Description: Returns the communicator of a remote program identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```const MPI_Comm* Manager_Comm_to(const MPMDManager* manager, ConnectionId id, IdType idType)```
    - Python: ```MPMDManager.comm_to(identifier: str | int) -> MPI.Comm```
  - Note: If the program with the specified ID is not found, an error message will be displayed, and the MPI environment will terminate using ```MPI_Abort()```.
- **Size Of**:
  - Description: Returns the size of a remote program identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```unsigned int Manager_Size_of(const MPMDManager* manager, ConnectionId id, IdType idType)```
    - Python: ```MPMDManager.size_of(identifier: str | int) -> int```
  - Note: If the program with the specified ID is not found, an error message will be displayed, and the MPI environment will terminate using ```MPI_Abort()```.

<h3 id="reference-multi-client">Multi World (Client)</h3>

- **Init**:
  - Description: Initializes the manager by setting its communicator and variables. This routine should only be called by processes that specifically intend to act as clients to servers, not by all processes in the program.
  - Syntax:
    - C: ```const ClientManager* Client_Init(uint maxServers)```
    - Python: ```ClientManager.init() -> None```
  - Note: In C, the function returns a pointer to the ```ClientManager```, which is required by other functions. Additionally, the maximum number of servers that can connect must be specified.
- **Finalize**:
  - Description: Finalizes the manager, freeing all allocated resources and terminating established connections.
  - Syntax:
    - C: ```void Client_Finalize(const ClientManager* manager)```
    - Python: ```ClientManager.finalize() -> None```
- **Connect**:
  - Description: Attempts to establish a connection with an open server that has the specified name. If successful, it returns the server's ```Intercommunicator```. Otherwise, an error message is displayed, and the MPI environment terminates using ```MPI_Abort()```. If the connection attempt fails, it retries after a delay, based on the attributes of the provided ConnectAttemptData structure/object, which includes: maxAttempts, initialWaitMs, waitIncrementMs e maxWaitMs. Default values are 10 maximum attempts, 500 ms initial wait, 500 ms increment per attempt, and 5000 ms maximum wait.
  - Syntax:
    - C: ```const MPI_Comm* Client_Connect(const ClientManager* manager, const char* serverName, const ConnectAttemptData* attemptData)```
    - Python: ```ClientManager.connect(server_name: str, attemptData = ConnectAttemptData()) -> MPI.Intercomm```
- **Retrieve Server Comm**:
  - Description: Returns the communicator of a server identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```const MPI_Comm* Client_Retrieve_Server_comm(const ClientManager* manager, ConnectionId id, IdType idType)```
    - Python: ```ClientManager.retrieve_server_comm(server_id: str | int) -> MPI.Intercomm```
    - Note: If the server with the specified ID is not found, an error message is displayed, and the MPI environment terminates using ```MPI_Abort()```.
- **Disconnect Server**:
  - Description: Disconnects and cleans up resources for the server identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```void Client_Disconnect_server(const ClientManager* manager, ConnectionId id, IdType idType)```
    - Python: ```ClientManager.disconnect_server(server_id: str | int) -> None```
    - Note: If the server with the specified ID is not found, an error message is displayed, and the MPI environment terminates using ```MPI_Abort()```.
- **Disconnect Servers**:
  - Description: Disconnects and cleans up resources for all previously connected servers.
  - Syntax:
    - C: ```void Client_Disconnect_servers(const ClientManager* manager)```
    - Python: ```ClientManager.disconnect_servers() -> None```

<h3 id="reference-multi-server">Multi World (Server)</h3>

- **Init**:
  - Description: Initializes the manager by setting its communicator and variables. If the server name is null, it will be automatically generated using the file name combined with the rank of the process that called the method, in that order. This routine should only be called by processes intending to act as servers and connect with clients, not by all processes in the program.
  - Syntax:
    - C: ```const ServerManager* Server_Init(char** argv, const char* serverName, uint maxClients)```
    - Python: ```ServerManager.init(server_name: str | None = None) -> None```
  - Note: In C, the function returns a pointer to the ```ServerManager```, which is required by other functions. Additionally, the maximum number of clients that can connect must be specified.
- **Finalize**:
  - Description: Finalizes the manager, freeing all allocated resources and terminating established connections.
  - Syntax:
    - C: ```void Server_Finalize(const ServerManager* manager)```
    - Python: ```ServerManager.finalize() -> None```
- **Local Name**:
  - Description: Returns the name of the local server.
  - Syntax:
    - C: ```const char* Server_Local_name(const ServerManager* manager)```
    - Python: ```ServerManager.local_name() -> str```
- **Open**:
  - Description: Opens the server for client connection requests by calling ```MPI_Open_port()``` and ```MPI_Publish_name()``` with the name provided during initialization.
  - Syntax:
    - C: ```void Server_Open(const ServerManager* manager)```
    - Python: ```ServerManager.open() -> None```
- **Close**:
  - Description: Closes the server by calling ```MPI_Unpublish_name()``` and ```MPI_Close_port()```. This routine is also called during the **Finalize** operation.
  - Syntax:
    - C: ```void Server_Close(const ServerManager* manager)```
    - Python: ```ServerManager.finalize() -> None```
- **Accept**:
  - Description: Waits for a client connection request and establishes a connection, returning the corresponding ```Intercommunicator```. Optionally, a name can be assigned to the connected client, which can be used as its identifier in other functions.
  - Syntax:
    - C: ```const MPI_Comm* Server_Accept(const ServerManager* manager, const char* clientName)```
    - Python: ```ServerManager.accept(client_id: str | None = None) -> Intercomm```
- **Retrieve Client Comm**:
  - Description: Returns the communicator of a client identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```const MPI_Comm* Server_Retrieve_Client_comm(const ServerManager* manager, ConnectionId id, IdType idType)```
    - Python: ```ServerManager.retrieve_client_comm(client_id: str | int) -> MPI.Intercomm```
    - Note: If the client with the specified ID is not found, an error message is displayed, and the MPI environment terminates using ```MPI_Abort()```.
- **Disconnect Client**:
  - Description: Disconnects and cleans up resources for the client identified by the provided ID, which can be its name or index.
  - Syntax:
    - C: ```void Server_Disconnect_client(const ServerManager* manager, ConnectionId id, IdType idType)```
    - Python: ```ServerManager.disconnect_client(client_id: str | int) -> None```
    - Note: If the client with the specified ID is not found, an error message is displayed, and the MPI environment terminates using ```MPI_Abort()```.
- **Disconnect Clients**:
  - Description: Disconnects and cleans up resources for all previously connected clients.
  - Syntax:
    - C: ```void Server_Disconnect_clients(const ServerManager* manager)```
    - Python: ```ServerManager.disconnect_clients() -> None```

<h2 id="license">:memo: License</h2>
This project is MIT Licensed. See <a href="https://github.com/RiadOliveira/MPMDManager/blob/main/LICENSE">LICENSE</a> file for more details.

<h2 id="authors">👨‍💻 Authors</h2>

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/RiadOliveira">
        <img src="https://avatars.githubusercontent.com/u/69125013?v=4" width="100px;" alt="Ríad Oliveira"/>
        <br/><sub><b>Ríad Oliveira</b></sub>
      </a>
    </td>
    <td align="center">
      <a href="https://github.com/paulohenriquels">
        <img src="https://avatars.githubusercontent.com/u/102553073?v=4" width="100px;" alt="Paulo Henrique"/>
        <br/><sub><b>Paulo Henrique</b></sub>
      </a>
    </td>
  </tr>
</table>

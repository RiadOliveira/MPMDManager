<h1 align="center">MPMDManager</h1>

<p align="center">
  An abstraction, built on top of the MPI library, that is designed to simplify the development of parallel programs in the MPMD context by providing functions to facilitate the identification, communication, and coordination of multiple parallel programs.
</p>

![image](https://img.shields.io/github/license/RiadOliveira/MPMDManager)

Contents
=================
   - [💡 Proposal](#proposal)
   - [🛠 Technologies](#technologies)
     - [Languages](#languages)
     - [Libraries](#libraries)
   - [:gear: Features](#features)
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

<h2 id="technologies">🛠 Technologies</h2>

- <h3 id="languages">Languages</h3>

    &nbsp;![Technologies](https://skillicons.dev/icons?i=c,python&theme=dark)

- <h3 id="libraries">Libraries</h3>

  - ![MPI (OpenMPI)](https://www.open-mpi.org/)
  - ![mpi4py](https://mpi4py.readthedocs.io/en/stable/)

<h2 id="features">:gear: Features</h2>

<h2 id="license">:memo: License</h2>
This project is MIT Licensed. See <a href="https://github.com/RiadOliveira/MPMDManager/blob/main/LICENSE">LICENSE</a> file for more details.

<h2 id="authors">👨‍💻 Authors</h2>

<table>
  <tr>
    <td align="center">
      <a href="https://github.com/RiadOliveira">
        <img src="https://avatars.githubusercontent.com/u/69125013?v=4" width="100px;" alt="Ríad Oliveira"/>
        <br /><sub><b>Ríad Oliveira</b></sub>
      </a>
    </td>
  </tr>
</table>

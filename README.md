# Regular language clasificator

This programm constructs a deterministic finite automata that recognizes a regular language defined by:
* a list of accepted words
* a list of rejected words
All words contain only lowercase letters of the English alphabet.

The automata is defined as a graph and begins as 3 unlinked states:
* a start state
* an accept state, the only final state of the DFA
* an error state

Each state is defined by:
* an ID
* a final flag, signaling if the state is either accept, reject or intermidiary
* a list of 26 nighbours, one for each possible letter

For each word in the accepted or rejected lists, a new chain is created in the graph, each transition in the chain is determined by the next unconsumed letter from the word. All chains start from the start state and end at either one the error or accept state, depending on the list the word was taken from. Chains can share prefixes, to minimize the number of states.

After construction, the graph is simplified. This is done by removing all states where all neighbours lead to one of the 2 final states.

At the end, each states gets an ID and the graph is returned as an adjacency matrix.

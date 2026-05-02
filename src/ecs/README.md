# Entity-Component-System v4

This version implements Entity-Component-Systems. Components inherit from the struct Component, Entity maintains an array with a fixed number of (possible) components, and Manager maintains a list of entities. We also have systems and they manager via the manager to ease access to them.

It also support groups of entities and handlers (assigning identifiers to entities for fast global access). Each entity belongs to a single group, resulting in a more efficient implementation.

It supports sending messages between systems. See messages_defs_examples.h for more information.



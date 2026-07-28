---
ns: CFX
apiset: server
---
## NETWORK_SET_ENTITY_OWNER

```c
BOOL NETWORK_SET_ENTITY_OWNER(Entity entity, Player player);
```

Transfers ownership (network control) of an entity to the specified player, but only when that player can actually take it. The reassignment is gated server-side by relevance: it succeeds only if the target player is in the same routing bucket as the entity, the entity is relevant to (i.e. streamed to) the target, and the target has actually instantiated the entity locally. If any of those conditions fail the ownership is left untouched and the native returns `false`.

This gating is deliberate: it prevents assigning ownership to a client that is not in scope of the entity, which would leave the entity owned by a client that has no game object for it (a "stuck" owner that cannot apply state, sync, or hand off control). Use [NETWORK_GET_ENTITY_OWNER](#_0x526FEE31) to read the current owner, and pair this with [SET_ENTITY_ORPHAN_MODE](#_0x489E9162) when you need the entity to persist while no relevant client is available.

## Parameters
* **entity**: The entity to transfer ownership of.
* **player**: The player (server ID) to make the new owner.

## Return value
`true` if ownership was transferred to (or already held by) the target player, `false` if the target could not take the entity (not relevant, not yet created, different routing bucket, invalid entity/player, or the entity is a player).

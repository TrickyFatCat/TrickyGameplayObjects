# About

**Tricky Gameplay Objects** is a plugin for Unreal Engine 5 that provides reusable components and interfaces for
managing common gameplay mechanics such as doors, chests, locks, buttons, and more. These systems are designed to
improve the workflow for developing interactive gameplay objects, making it easier to handle complex state transitions,
interactions, and key/lock systems.

## Features

- **State Management Components**:
    - `DoorStateController`: Controls door states such as opened, closed, locked, disabled.
    - `ChestStateController`: Controls chest states such as opened, closed, locked, disabled.
    - `LockStateController`: Controls lock states such as locked, unlocked, disabled.
    - `ButtonStateController`: Controls button states such as pressed, released, disabled.
    - `GameplayObjectStateController`: Controls generic gameplay objects states such as active, inactive, disabled.

- **Blueprint Integration**:
    - All components and interfaces are fully exposed to Blueprints.
    - Seamless integration allows for rapid prototyping and easier customization.

- **Simple LockKey System**:
    - `KeyringComponent`: A reusable component for managing acquired lock keys (adding, removing, using keys).
    - `LockKeyType`: A class representing key types used to interact with locks, chests, doors, and other objects.

## Installation

1. Copy the plugin folder to your project's `Plugins` directory.
2. Open your Unreal Engine project.
3. Enable the `TrickyGameplayObjects` plugin in the Plugins menu.
4. Restart the editor.

## Functionality Overview

All components have a number of properties, functions and delegates which can be used to implement them.

### DoorStateController

#### Properties:

1. **`InitialState`**
    - Defines the initial state of the door during component initialization.
    - Cannot be set to `Transition`.

2. **`CurrentState`**
    - Tracks the door's current operational state.

3. **`TargetState`**
    - Represents the state the door is transitioning to.

4. **`LastState`**
    - Holds the previous state of the door before the last state transition.

#### Functions:

1. **`OpenDoor(bool bTransitImmediately)`**  
   Attempts to open the door.
    - `bTransitImmediately`: Determines if the door should switch to the open state immediately.
    - Returns `true` if successful.

2. **`CloseDoor(bool bTransitImmediately)`**  
   Attempts to close the door.
    - `bTransitImmediately`: Determines if the door should switch to the closed state immediately.
    - Returns `true` if successful.

3. **`LockDoor(bool bTransitImmediately)`**  
   Attempts to lock the door.
    - The door must be in the `Closed` state to lock.
    - `bTransitImmediately`: Determines if the door should switch to the locked state immediately.
    - Returns `true` if successful.

4. **`UnlockDoor(bool bTransitImmediately)`**  
   Attempts to unlock the door.
    - The door must be in the `Locked` state to unlock.
    - `bTransitImmediately`: Determines if the door should switch to the unlocked state immediately.
    - Returns `true` if successful.

5. **`DisableDoor(bool bTransitImmediately)`**  
   Disables the door, putting it in a non-interactable state.
    - `bTransitImmediately`: Determines if the door should immediately transition to the disabled state.
    - Returns `true` if successful.

6. **`EnableDoor(bool bTransitImmediately)`**  
   Re-enables a disabled door, restoring it to its last state (`LastState`).
    - `bTransitImmediately`: Determines if the door should immediately transition to its restored state.
    - Returns `true` if successful.

7. **`ForceDoorState(EDoorState NewState, bool bTransitImmediately)`**  
   Forces the door into a specific state, skipping normal state checks.
    - `NewState`: The desired state (`Opened`, `Closed`, etc.).
    - `bTransitImmediately`: Determines if the transition should occur immediately.
    - Returns `true` if successful.

8. **`FinishDoorStateTransition()`**  
   Completes a state transition if the door is currently in the `Transition` state.
    - Returns `true` if the transition is completed.

9. **`ReverseDoorStateTransition()`**  
   Reverses a state transition, swapping `TargetState` and `LastState`.
    - Returns `true` if successful.

#### Delegates:

1. **`OnDoorStateChanged`**  
   Triggered when the `CurrentState` changed.
    - Parameters: `Component`, `NewState`, `bChangedImmediately`.

2. **`OnDoorStateTransitionStarted`**  
   Triggered when the door enters the `Transition` state.
    - Parameters: `Component`, `TargetState`.

3. **`OnDoorStateTransitionFinished`**  
   Triggered when the door exits the `Transition` state and reaches its `TargetState`.
    - Parameters: `Component`, `NewState`.

4. **`OnDoorStateTransitionReversed`**  
   Triggered when a state transition is reversed (swapping `TargetState` and `LastState`).
    - Parameters: `Component`, `NewTargetState`.

### Chest State Controller

#### Properties:

1. **`InitialState`**
    - Defines the initial state of the chest during component initialization.
    - Cannot be set to `Transition`.

2. **`CurrentState`**
    - Tracks the chest's current operational state.

3. **`TargetState`**
    - Represents the state the chest is transitioning to.

4. **`LastState`**
    - Holds the previous state of the chest before the last state transition.

#### Functions:

1. **`OpenChest(bool bTransitImmediately)`**  
   Attempts to open the chest.
    - `bTransitImmediately`: Determines if the chest should switch to the open state immediately.
    - Returns `true` if successful.

2. **`CloseChest(bool bTransitImmediately)`**  
   Attempts to close the chest.
    - `bTransitImmediately`: Determines if the chest should switch to the closed state immediately.
    - Returns `true` if successful.

3. **`LockChest(bool bTransitImmediately)`**  
   Attempts to lock the chest.
    - The chest must be in the `Closed` state to lock.
    - `bTransitImmediately`: Determines if the chest should switch to the locked state immediately.
    - Returns `true` if successful.

4. **`UnlockChest(bool bTransitImmediately)`**  
   Attempts to unlock the chest.
    - The chest must be in the `Locked` state to unlock.
    - `bTransitImmediately`: Determines if the chest should switch to the unlocked state immediately.
    - Returns `true` if successful.

5. **`DisableChest(bool bTransitImmediately)`**  
   Disables the chest, putting it in a non-interactable state.
    - `bTransitImmediately`: Determines if the chest should immediately transition to the disabled state.
    - Returns `true` if successful.

6. **`EnableChest(bool bTransitImmediately)`**  
   Re-enables a disabled chest, restoring it to its last state (`LastState`).
    - `bTransitImmediately`: Determines if the chest should immediately transition to its restored state.
    - Returns `true` if successful.

7. **`ForceChestState(EChestState NewState, bool bTransitImmediately)`**  
   Forces the chest into a specific state, skipping normal state checks.
    - `NewState`: The desired state (`Opened`, `Closed`, etc.).
    - `bTransitImmediately`: Determines if the transition should occur immediately.
    - Returns `true` if successful.

8. **`FinishChestStateTransition()`**  
   Completes a state transition if the chest is currently in the `Transition` state.
    - Returns `true` if the transition is completed.

9. **`ReverseChestStateTransition()`**  
   Reverses a state transition, swapping `TargetState` and `LastState`.
    - Returns `true` if successful.

#### Delegates:

1. **`OnChestStateChanged`**  
   Triggered when the `CurrentState` changed.
    - Parameters: `Component`, `NewState`, `bChangedImmediately`.

2. **`OnChestStateTransitionStarted`**  
   Triggered when the chest enters the `Transition` state.
    - Parameters: `Component`, `TargetState`.

3. **`OnChestStateTransitionFinished`**  
   Triggered when the chest exits the `Transition` state and reaches its `TargetState`.
    - Parameters: `Component`, `NewState`.

4. **`OnChestStateTransitionReversed`**  
   Triggered when a state transition is reversed (swapping `TargetState` and `LastState`).
    - Parameters: `Component`, `NewTargetState`.

### Lock State Controller

#### Properties:

1. **`InitialState`**
    - Defines the initial state of the lock during component initialization.
    - Cannot be set to `Transition`.

2. **`CurrentState`**
    - Tracks the lock's current operational state.

3. **`TargetState`**
    - Represents the state the lock is transitioning to.

4. **`LastState`**
    - Holds the previous state of the lock before the last state transition.

#### Functions:

1. **`Lock(bool bTransitImmediately)`**  
   Attempts to lock the lock.
    - The lock must be in the `Unlocked` state to lock.
    - `bTransitImmediately`: Determines if the lock should switch to the locked state immediately.
    - Returns `true` if successful.

2. **`Unlock(bool bTransitImmediately)`**  
   Attempts to unlock the lock.
    - The lock must be in the `Locked` state to unlock.
    - `bTransitImmediately`: Determines if the lock should switch to the unlocked state immediately.
    - Returns `true` if successful.

3. **`DisableLock(bool bTransitImmediately)`**  
   Disables the lock, putting it in a non-interactable state.
    - `bTransitImmediately`: Determines if the lock should immediately transition to the disabled state.
    - Returns `true` if successful.

4. **`EnableLock(bool bTransitImmediately)`**  
   Re-enables a disabled lock, restoring it to its last state (`LastState`).
    - `bTransitImmediately`: Determines if the lock should immediately transition to its restored state.
    - Returns `true` if successful.

5. **`ForceLockState(ELockState NewState, bool bTransitImmediately)`**  
   Forces the lock into a specific state, skipping normal state checks.
    - `NewState`: The desired state (`Locked`, `Unlocked`, etc.).
    - `bTransitImmediately`: Determines if the transition should occur immediately.
    - Returns `true` if successful.

6. **`FinishLockStateTransition()`**  
   Completes a state transition if the lock is currently in the `Transition` state.
    - Returns `true` if the transition is completed.

7. **`ReverseLockStateTransition()`**  
   Reverses a state transition, swapping `TargetState` and `LastState`.
    - Returns `true` if successful.

#### Delegates:

1. **`OnLockStateChanged`**  
   Triggered when the `CurrentState` changed.
    - Parameters: `Component`, `NewState`, `bChangedImmediately`.

2. **`OnLockStateTransitionStarted`**  
   Triggered when the lock enters the `Transition` state.
    - Parameters: `Component`, `TargetState`.

3. **`OnLockStateTransitionFinished`**  
   Triggered when the lock exits the `Transition` state and reaches its `TargetState`.
    - Parameters: `Component`, `NewState`.

4. **`OnLockStateTransitionReversed`**  
   Triggered when a state transition is reversed (swapping `TargetState` and `LastState`).
    - Parameters: `Component`, `NewTargetState`.

### Button State Controller

#### Properties:

1. **`InitialState`**
   - Defines the initial state of the button during component initialization.
   - Cannot be set to `Transition`.

2. **`CurrentState`**
   - Tracks the button's current operational state.

3. **`TargetState`**
   - Represents the state the button is transitioning to.

4. **`LastState`**
   - Holds the previous state of the button before the last state transition.

#### Functions:

1. **`PressButton(bool bTransitImmediately)`**  
   Attempts to press the button.
   - The button must be in the `Released` state to press.
   - `bTransitImmediately`: Determines if the button should switch to the pressed state immediately.
   - Returns `true` if successful.

2. **`ReleaseButton(bool bTransitImmediately)`**  
   Attempts to release the button.
   - The button must be in the `Pressed` state to release.
   - `bTransitImmediately`: Determines if the button should switch to the released state immediately.
   - Returns `true` if successful.

3. **`DisableButton(bool bTransitImmediately)`**  
   Disables the button, putting it in a non-interactable state.
   - `bTransitImmediately`: Determines if the button should immediately transition to the disabled state.
   - Returns `true` if successful.

4. **`EnableButton(bool bTransitImmediately)`**  
   Re-enables a disabled button, restoring it to its last state (`LastState`).
   - `bTransitImmediately`: Determines if the button should immediately transition to its restored state.
   - Returns `true` if successful.

5. **`ForceButtonState(EButtonState NewState, bool bTransitImmediately)`**  
   Forces the button into a specific state, skipping normal state checks.
   - `NewState`: The desired state (`Pressed`, `Released`, etc.).
   - `bTransitImmediately`: Determines if the transition should occur immediately.
   - Returns `true` if successful.

6. **`FinishButtonStateTransition()`**  
   Completes a state transition if the button is currently in the `Transition` state.
   - Returns `true` if the transition is completed.

7. **`ReverseButtonStateTransition()`**  
   Reverses a state transition, swapping `TargetState` and `LastState`.
   - Returns `true` if successful.

#### Delegates:

1. **`OnButtonStateChanged`**  
   Triggered when the `CurrentState` changed.
   - Parameters: `Component`, `NewState`, `bChangedImmediately`.

2. **`OnButtonStateTransitionStarted`**  
   Triggered when the button enters the `Transition` state.
   - Parameters: `Component`, `TargetState`.

3. **`OnButtonStateTransitionFinished`**  
   Triggered when the button exits the `Transition` state and reaches its `TargetState`.
   - Parameters: `Component`, `NewState`.

4. **`OnButtonStateTransitionReversed`**  
   Triggered when a state transition is reversed (swapping `TargetState` and `LastState`).
   - Parameters: `Component`, `NewTargetState`.

### Gameplay Object State Controller

#### Properties:

1. **`InitialState`**
   - Defines the initial state of the gameplay object during component initialization.
   - Cannot be set to `Transition`.

2. **`CurrentState`**
   - Tracks the gameplay object's current operational state.

3. **`TargetState`**
   - Represents the state the gameplay object is transitioning to.

4. **`LastState`**
   - Holds the previous state of the gameplay object before the last state transition.

#### Functions:

1. **`ActivateGameplayObject(bool bTransitImmediately)`**  
   Attempts to activate the gameplay object.
   - The gameplay object must be in the `Inactive` state to press.
   - `bTransitImmediately`: Determines if the gameplay object should switch to the active immediately.
   - Returns `true` if successful.

2. **`DeactivateGameplayObject(bool bTransitImmediately)`**  
   Attempts to deactivate the gameplay object.
   - The gameplay object must be in the `Active` state to release.
   - `bTransitImmediately`: Determines if the gameplay object should switch to the inactive state immediately.
   - Returns `true` if successful.

3. **`DisableGameplayObject(bool bTransitImmediately)`**  
   Disables the gameplay object, putting it in a non-interactable state.
   - `bTransitImmediately`: Determines if the gameplay object should immediately transition to the disabled state.
   - Returns `true` if successful.

4. **`EnableGameplayObject(bool bTransitImmediately)`**  
   Re-enables a disabled gameplay object, restoring it to its last state (`LastState`).
   - `bTransitImmediately`: Determines if the gameplay object should immediately transition to its restored state.
   - Returns `true` if successful.

5. **`ForceGameplayObjectState(EGameplayObjectState NewState, bool bTransitImmediately)`**  
   Forces the gameplay object into a specific state, skipping normal state checks.
   - `NewState`: The desired state (`Active`, `Inactive`, etc.).
   - `bTransitImmediately`: Determines if the transition should occur immediately.
   - Returns `true` if successful.

6. **`FinishGameplayObjectStateTransition()`**  
   Completes a state transition if the gameplay object is currently in the `Transition` state.
   - Returns `true` if the transition is completed.

7. **`ReverseGameplayObjectStateTransition()`**  
   Reverses a state transition, swapping `TargetState` and `LastState`.
   - Returns `true` if successful.

#### Delegates:

1. **`OnGameplayObjectStateChanged`**  
   Triggered when the `CurrentState` changed.
   - Parameters: `Component`, `NewState`, `bChangedImmediately`.

2. **`OnGameplayObjectStateTransitionStarted`**  
   Triggered when the gameplay object enters the `Transition` state.
   - Parameters: `Component`, `TargetState`.

3. **`OnGameplayObjectStateTransitionFinished`**  
   Triggered when the gameplay object exits the `Transition` state and reaches its `TargetState`.
   - Parameters: `Component`, `NewState`.

4. **`OnGameplayObjectStateTransitionReversed`**  
   Triggered when a state transition is reversed (swapping `TargetState` and `LastState`).
   - Parameters: `Component`, `NewTargetState`.

### Keyring Component

#### Properties:

1. **`AcquiredKeys`**
   - Stores an array of lock key types (`TSubclassOf<ULockKeyType>`) that have been acquired by the component.
   - This property is maintained internally and is not directly exposed to modify. Use provided functions to manage the keyring.

#### Functions:

1. **`GetAcquiredKeys`**
   - Retrieves the current list of acquired lock keys.
   - Returns `AcquiredLockKeys`: An array of acquired lock key types.

2. **`AddLockKey`**
   - Adds a new lock key to the component, ensuring uniqueness.
   - `LockKey`: The lock key type to add.
   - Returns `true` if the key was added successfully, `false` otherwise.

3. **`RemoveLockKey`**
   - Removes a specific lock key type from the component.
   - `LockKey`: The lock key type to remove.
   - Returns `true` if the key was removed successfully, `false` otherwise.

4. **`RemoveAllLockKeys`**
   - Clears all acquired lock keys from the component.
   - Returns `true` if all keys were successfully removed, `false` otherwise.

5. **`UseLockKey`**
   - Attempts to use a specific lock key from the component.
   - `LockKey`: The lock key type to use.
   - Returns `true` if the key was successfully used, `false` otherwise.

6. **`HasLockKey`**
   - Checks if the specified lock key type exists in the component.
   - `LockKey`: The lock key type to check.
   - Returns `true` if the key is present, `false` otherwise.

#### Delegates:

1. **`OnLockKeyAdded`**
   - Triggered after a new lock key is successfully added to the component.
   - Parameters: `Component`, `LockKey`.

2. **`OnLockKeyRemoved`**
   - Triggered when a lock key is successfully removed from the component.
   - Parameters: `Component`, `LockKey`.

3. **`OnAllLockKeysRemoved`**
   - Triggered when all lock keys are removed from the component.
   - Parameters: `Component`.

4. **`OnLockKeyUsed`**
   - Triggered when a lock key is successfully used by the component.
   - Parameters: `Component`, `LockKey`.  
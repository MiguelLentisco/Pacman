# [Pacman](https://github.com/MiguelLentisco/Pacman/)
 Replicated original Pacman in 48h with Unreal Engine 4.27.2.
 
# Summary
 The classic Pacman where in order to win you have to eat all the dots and the pills. But be careful with the ghosts because if they touch you, you'll lose a life and you don't have that many! But do not worry as the pills are PowerUps that allow you to eat them without any trouble. You can move Pacman with WASD or Arrows.
 
 Go ahead and try to obtain a high score!
 
 # Code
 
 The project's code structure is as it follows:
  - Actors
    - Dot : the simple dot that Pacman eats
    - OutsideSpawnTrigger : Trigger used to change the ghost collision in order to block the spawn exit
    - Pawn :
      - BasePawn : basic pawn functionality, common to ghosts and Pacman
      - Diretion : basic enum for directions
      - PawnMovementComponent : physical movement's logic
      - GhostPawn : the ghost's main logic (IA, game events, interaction)
      - PacmanPawn : Pacman's main logic (game events, interaction)
      - PacmanController : binds pawn's input
    - PointsShower : actor used to spawn the points effect sprite
    - PowerUp : the pill that gives power up to Pacman
    - Teleport : trigger that teleports the pawns to the other side of the level
   - Core
     - AudioSubsystem : handles the fx and background audio
     - GameMode : set initial lifes
     - GameState : handles the win/loose conditions
     - MessagingSubsystem :
       - GameplayEvents : 
       - Message : base and basic messages clases to sent with the events
       - MessagingSubsystem: handles sending events with messages through the game
     - PlayerState : manages Pacman's current lifes and points
   - Data
     -  EatableData : data table row with eatable's points and music
   - Interfaces :
     - EatableInterface : react to interaction with Pacman, common logic
   - Settings
     -  PacmanSettings : various settings used in the game
 
 # Content
 Basic description of the project's organization:
 
 - Art : all the textures, materials and sprites used in the game divided by actor.
 - Audio : all the fx and backgroud audios
 - Data : eatable data table (points and FX by item)
 - Levels : MainLevel and Menu
 - Logic : BP of all the C++ logic and the UI

# External assets

- Sprite assets from [mitxela](https://mitxela.com/projects/console/pacman)
- Music from [SoundsResource](https://www.sounds-resource.com/)

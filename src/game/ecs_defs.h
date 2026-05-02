// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

// Components list - must have at least one element
//
struct Transform;
struct Image;
struct PacManComponent;
struct ImageWithFrames;
struct FruitComponent;

#define _CMPS_LIST_ \
	Transform, \
	Image, \
	PacManComponent, \
	ImageWithFrames, \
	FruitComponent

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	FRUITS, \
	GHOSTS

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    PACMAN

// Systems list - must have at least one element
//
class RenderSystem;
class PacManSystem;
class GameCtrlSystem;
class CollisionsSystem;
class FoodSystem;
class GhostSystem;
class ImmunitySystem;

#define _SYS_LIST_ \
	RenderSystem, \
	PacManSystem, \
	GameCtrlSystem, \
	CollisionsSystem, \
	FoodSystem, \
	GhostSystem, \
	ImmunitySystem 


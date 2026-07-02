This is a small C++ game runtime prototype.
It currently supports entity creation, transform and velocity components,
a simple world update loop, and basic performance measurement.

# MiniGameRuntime

## Overview

## Current Features

## Project Structure

## Build Instructions

## Run Example

## Current Output

## Architecture

## Learning Notes

## Roadmap

【 1. 基础物质（Components） 】 ---> 纯结构体，只存数据
   ├── TransformComponent (存 position)
   ├── VelocityComponent  (存 velocity)
   └── AABBComponent      (存 halfExtents)
          |
          v
  【 2. 核心大账本（World） 】 --------> 负责管账（增、删、查）
   ├── CreateEntity() / DestoryEntity()
   ├── AddTransform() / GetTransform() / HasTransform()
   ├── AddVelocity()  / GetVelocity()  / HasVelocity()
   └── AddAABB()      / GetAABB()      / HasAABB()
          |
          v
  【 3. 自动化流水线（Systems） 】 ----> 负责干活（只管逻辑，不存数据）
   ├── World::Update() 或 MovementSystem (把 Transform + Velocity 拿出来算位移)
   └── CollisionSystem                  (把 Transform + AABB 拿出来算碰撞)
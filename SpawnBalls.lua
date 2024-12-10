function SpawnBalls(numberOfBalls, ballSize)
    -- Loop to create enemies
   -- Call the C++ CreateEnemy function with a unique ID
   for i = 1, numberOfBalls do
    -- Call the C++ CreateEnemy function with a unique ID
    CreateEnemy(i, ballSize)
end
    
end

-- Actually spawn the balls
-- You can adjust the number of balls as needed
SpawnBalls(1, 0.5)  -- this wil create manny
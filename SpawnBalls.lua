function SpawnBalls(numberOfBalls)
    -- Loop to create enemies
   -- Call the C++ CreateEnemy function with a unique ID
   for i = 1, numberOfBalls do
    -- Call the C++ CreateEnemy function with a unique ID
    CreateEnemy(i)
end
    
end

-- Actually spawn the balls
-- You can adjust the number of balls as needed
SpawnBalls(3)  -- This will create 5 enemies with IDs 1, 2, 3, 4, and 5
module Main where

	main :: IO()
	scalarproduct a b | length a == length b = sum (zipWith (*) a b) | otherwise = error "Vector sizes must match"
	main = print $ scalarproduct [1,2,3] [3,2,1]
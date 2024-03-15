function squares()
	for i = 2, 3 do
		coroutine.yield(i * i)
	end
end

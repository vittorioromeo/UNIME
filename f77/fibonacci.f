c come fare return x?
c long integer
c parametri come puntatori? come copiare?

	program fibonacci_bello
	character*32 text

	call fibonacci(10)
c	call factorial(6)

	end

c 	`n` viene passato come puntatore
c 	se devo modificare `n`, devo prima copiarlo
	subroutine factorial(n)
c 		specifico il tipo del parametro `n`
		integer n

c 		dichiaro variabili locali
		integer iteration_count
		integer result
		character*64 buffer
		
c 		esegui operazioni
		iteration_count = n
		result = 1

		do while (iteration_count >= 2)
			result = result * iteration_count
			iteration_count = iteration_count - 1
		enddo

		write (buffer, *) 'factorial'
		write (buffer(12:), '(i6)') n
		write (buffer(20:), *) '='
		write (buffer(26:), '(i10)') result
		write (*, *) buffer
	end

	subroutine fibonacci(iteration_count)
		integer iteration_count

		integer i0
		integer i1
		integer i2
		integer current_iteration
		character*64 buffer

		i0 = 1
		i1 = 1
		current_iteration = 0

		do while (current_iteration < iteration_count + 1)
			write (buffer, *) 'fibonacci'
			write (buffer(12:), '(i6)') current_iteration
			write (buffer(20:), *) '='
			write (buffer(26:), '(i10)') i0
			write (*, *) buffer

			i2 = i0 + i1
			i0 = i1
			i1 = i2
			current_iteration = current_iteration + 1
		enddo
	end

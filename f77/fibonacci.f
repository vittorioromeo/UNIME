c come fare return x?
c long integer
c parametri come puntatori? come copiare?

	program fibonacci_bello
	character*32 text

c	call fibonacci(10)
	real*8 v 
	v = 35.0

	call factorial(v)

	end

c 	`n` viene passato come puntatore
c 	se devo modificare `n`, devo prima copiarlo
	subroutine factorial(n)
c 		specifico il tipo del parametro `n`
		real*8 n

c 		dichiaro variabili locali
		real*8 iteration_count
		real*8 result
		
c 		esegui operazioni
		iteration_count = n
		result = 1

		do while (iteration_count >= 2)
			result = result * iteration_count
			iteration_count = iteration_count - 1
		enddo

		write (*, *) 'factorial', n, ' = ', result
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
c 			write (buffer, *) 'fibonacci'
c 			write (buffer(12:), '(i6)') current_iteration
c 			write (buffer(20:), *) '='
c 			write (buffer(26:), '(i10)') i0
c 			write (*, *) buffer

			write (*, *) 'fibonacci', current_iteration, ' = ', i0

			i2 = i0 + i1
			i0 = i1
			i1 = i2
			current_iteration = current_iteration + 1
		enddo
	end

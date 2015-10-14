program hello_world3
implicit none
character*32 text

text = 'Hello World'

call printtext(text)
write (*,*) text

end

subroutine printtext(tekst)
implicit none
character*32 tekst

tekst(1:5) = 'ciao'
write (*,*) tekst

end

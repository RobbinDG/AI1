% isnumber(X) is true if X is a isnumber

isnumber(0).
isnumber(s(X)) :- isnumber(X).

% plus(X,Y,Z) is true if X + Y = Z

plus(0,X,X) :- isnumber(X).
plus(s(X),Y,s(Z)) :- plus(X,Y,Z).

% minus(X,Y,Z) is true if X - Y =Z

minus(X,0,X) :- isnumber(X).
minus(s(X),s(Y),Z) :- minus(X,Y,Z).

% times(X,Y,Z) is true if X * Y = Z

times(X,0,0) :- isnumber(X).
times(X,s(Y),Z) :- times(X,Y,Z1), plus(X,Z1,Z).

% pow(X,Y,Z) is true if X^Y = Z

pow(X,0,s(0)) :- isnumber(X).
pow(X,s(Y),Z) :- pow(X,Y,Z1), times(X,Z1,Z).

% even(N) is true if N mod 2 = 0
even(N) :- N mod 2 =:= 0. 

% odd(N) is true if N mod 2 = 1
odd(N) :- N mod 2 =:= 1.

% div2(N,D) is true if N / 2 = D
div2(N,D) :- N / 2 =:= D.

% divi2(N,D) is true if N / 2 = D
divi2(N,D) :- times(s(s(0)),D,N).

% log(X,B,N) is true if B^N = X
log(1,_,0) :- !.
log(X,B,N) :-
X > 1,
X1 is X/B,
N1 is N - 1,
log(X1, B, N1).

% fib(X,Y) is true if fib(X)=Y [Note: fib(0)=0, fib(1)=1,
% fib(n) = fib(n-1) + fib(n-2)]
fib(0,0) :- !.
fib(1,1) :- !.
fib(X,Y) :- 
X > 1,  
X1 is X-1, 
X2 is X-2,
fib(X1, Y1), 
fib(X2, Y2), 
Y is Y1+Y2.

% power(X,N,Y) is true if X^N = Y (using improg tactics)
power(_,0,1) :- !.
power(X,1,X) :- !.
power(X,N,Y) :- N>1, Z is N mod 2, power(X,N,Y,Z).
power(_,0,1,_) :- !.
power(X,1,X,_) :- !.
power(X,N,Y,0) :- N>1, N1 is N/2, X1 is X*X, power(X1,N1,Y).
power(X,N,Y,1) :- N>1, N1 is N-1, Y1 is Y/X, power(X,N1,Y1,0).

% Example queries:
% Isnumbers are represented as successors of 0. For example, 2 is s(s(0)).
% 2+2=4 is plus(s(s(0)), s(s(0)), s(s(s(s(0)))))
% 3*2=? is times(s(s(s(0))), s(s(0)), X)

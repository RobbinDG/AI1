len([],0).
len([H|T],N) :- len(T,N1), N is N1+1.

% member(X,L) is true if X is a member of list L
member(X, [H|T]):- X = H; member(X, T).

% concat(L,X,Y) is true if L is the concatenation of lists X and Y
concat(L,X,Y) :- append(X,Y,R), R = L.

% reverse(L,R) is true if R is the reversal of the list L
reverse([],[]).
reverse([H|T], R) :- reverse(T, T1), append(T1, [H], R).

% palindrome(L) is true if L is a palindrome
% palindrome([]).
% palindrome([_]).
% palindrome(L) :- append([H|T], [H], L), palindrome(T).
palindrome(L):- reverse(L, L).
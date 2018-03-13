a = 1.234; b=2.345;
tic;
for i=1:100000000
    a = a+b;
    b = a-b;
    b = a-b;
end
fprintf('(+,-) time:%f\n',toc/3);
tic;
for i=1:100000000
    a = a*b;
    b = a/b;
    b = a/b;
end
fprintf('(*,/) time:%f\n',toc/3);
tic;
for i=1:100000000
    a = sin(b);
    b = sin(a);
end
fprintf('(sin) time:%f\n',toc/2);
tic;
for i=1:100000000
    a = randi(32768,1);
end
fprintf('(sin) time:%f\n',toc);
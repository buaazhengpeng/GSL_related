
load rk4.txt
load rk4imp.txt
load gear1.txt
load gear2.txt
plot(rk4(:,1),rk4(:,2),'b');hold on;
plot(rk4imp(:,1),rk4imp(:,2),'g');hold on;
plot(gear1(:,1),gear1(:,2),'r');hold on;
plot(gear2(:,1),gear2(:,2),'p');hold on;


plot(rk4(:,1),rk4(:,3),'b');hold on;
plot(rk4imp(:,1),rk4imp(:,3),'m');hold on;
plot(gear1(:,1),gear1(:,3),'r');hold on;
plot(gear2(:,1),gear2(:,3),'k');hold on;

E:
-980*cos(rk4(:,2))+0.5*rk4(:,3).^2
-980*cos(rk4imp(:,2))+0.5*rk4imp(:,3).^2
-980*cos(gear1(:,2))+0.5*gear1(:,3).^2
-980*cos(gear2(:,2))+0.5*gear2(:,3).^2

plot(rk4(:,1),-980*cos(rk4(:,2))+0.5*rk4(:,3).^2,'b');hold on;
plot(rk4imp(:,1),-980*cos(rk4imp(:,2))+0.5*rk4imp(:,3).^2,'m');hold on;
plot(gear1(:,1),-980*cos(gear1(:,2))+0.5*gear1(:,3).^2,'r');hold on;
plot(gear2(:,1),-980*cos(gear2(:,2))+0.5*gear2(:,3).^2,'k');hold on;

t=0:0.001:20;
w=sqrt(980);
y=pi*cos(w*t);
plot(t,y,'k')
syms x;
f=sin(x);
T=taylor(f,x,0,'order',10)
%¶¯»­ÑÝÊ¾
x=fp;t=fp(:,1);
rA1(:,1)=x(:,4); %ºá×ø±ê
rA1(:,2)=x(:,6); %×Ý×ø±ê
e=0.1;
rA2(:,1)=x(:,4)+e*sin(x(:,2)); %ºá×ø±ê
rA2(:,2)=x(:,6)-e*cos(x(:,2)); %×Ý×ø±ê

writerObj=VideoWriter('donghua');
open(writerObj);
figure(1); hold on; axis equal;% ÉèÖÃ×ø±êÖá·¶Î§ 
%set(gca,'ydir','reverse')
ROD2=plot([rA1(1,2),rA2(1,2)],[rA1(1,1),rA2(1,1)],'b','linewidth',4);

JORN2=plot(rA1(1,2),rA1(1,1),'o','markersize',10);
JORN3=plot(rA1(1,2),rA1(1,1),'o','markersize',100000);
axis([-1,1,-0.1,0.2]); 
k=1;
while k<=size(t,1);
    JORN2=plot(rA1(k,2),rA1(k,1),'o','markersize',10);
    JORN3=plot(rA1(k,2),rA1(k,1),'o','markersize',100000);
    set(ROD2,'xdata',[rA1(k,2),rA2(k,2)],'ydata',[rA1(k,1),rA2(k,1)]);hold on;
    pause(0.05);
    k=k+1;
     %axis([-1,1,-1.5,2]); 
    
    
    drawnow;   
    F=getframe;
    writeVideo(writerObj,F);   
end
close(writerObj);
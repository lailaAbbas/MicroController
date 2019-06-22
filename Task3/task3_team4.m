function varargout = task3_team4(varargin)
% TASK3_TEAM4 MATLAB code for task3_team4.fig
%      TASK3_TEAM4, by itself, creates a new TASK3_TEAM4 or raises the existing
%      singleton*.
%
%      H = TASK3_TEAM4 returns the handle to a new TASK3_TEAM4 or the handle to
%      the existing singleton*.
%
%      TASK3_TEAM4('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TASK3_TEAM4.M with the given input arguments.
%
%      TASK3_TEAM4('Property','Value',...) creates a new TASK3_TEAM4 or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before task3_team4_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to task3_team4_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help task3_team4

% Last Modified by GUIDE v2.5 24-Oct-2018 16:02:34

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @task3_team4_OpeningFcn, ...
                   'gui_OutputFcn',  @task3_team4_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT
end

% --- Executes just before task3_team4 is made visible.
function task3_team4_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to task3_team4 (see VARARGIN)

% Choose default command line output for task3_team4
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes task3_team4 wait for user response (see UIRESUME)
% uiwait(handles.figure1);
axes(handles.axes3)
grid on;
end

% --- Outputs from this function are returned to the command line.
function varargout = task3_team4_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;
end


% --- Executes on button press in plot.
function plot_Callback(hObject, eventdata, handles)
% hObject    handle to plot (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

global x y
axes(handles.axes3)
cla(handles.axes3);
xlim([0 200]);              % range of x axis from min to max point
ylim([0 140]);              % range of y axis from min to max point
[X,Y]=ginput;               % enter points by click on plot
x=[];
y=[];
for i = 1:length(X)-1
    [xx,yy]=fillline([X(i),Y(i)],[X(i+1),Y(i+1)],10);
    x=[x,xx];
    y=[y,yy];
end
 x=floor(x)
 y=floor(y)
plot(x,y , '-o')
xlim([0 200])
ylim([0 140])
grid on;

sendPoints(hObject, eventdata, handles);

% Update handles structure
guidata(hObject, handles);
end

% take starting, ending point & number of points in between
% make line to connect between 2 coordinates
function [xx,yy]=fillline(startp,endp,pts)
        m=(endp(2)-startp(2))/(endp(1)-startp(1)); %gradient 
        if m==Inf %vertical line
            xx(1:pts)=startp(1);
            yy(1:pts)=linspace(startp(2),endp(2),pts);     %Generate linearly spaced vector
        elseif m==0 %horizontal line
            xx(1:pts)=linspace(startp(1),endp(1),pts);
            yy(1:pts)=startp(2);
        else %if (endp(1)-startp(1))~=0
            xx=linspace(startp(1),endp(1),pts);
            yy=m*(xx-startp(1))+startp(2);
        end
end

% --- Executes on button press in Connect.
function Connect_Callback(hObject, eventdata, handles)
% hObject    handle to Connect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
handles.bt = Bluetooth('HC-06', 1);
fopen(handles.bt);
set(handles.text,'String','Connected');
display('connected');
% Update handles structure
guidata(hObject, handles);
end


% --- Executes on button press in DisConnect.
function DisConnect_Callback(hObject, eventdata, handles)
% hObject    handle to DisConnect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
fclose(handles.bt);
set(handles.text,'String','Disconnected!!');
display('Disconnected');
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function sendPoints(hObject, eventdata, handles)
global x y
str = '';
for i = 1:length(x)
    str=[str,num2str(x(i))];
    str = [str ,'x'];
    str=[str,num2str(y(i))];
    str = [str ,'y'];
end
str = [str ,'e']
fwrite(handles.bt,str);
q=fscanf(handles.bt, '%d')
end

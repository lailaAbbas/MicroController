function varargout = SML_wifi_app(varargin)
% SML_WIFI_APP MATLAB code for SML_wifi_app.fig
%      SML_WIFI_APP, by itself, creates a new SML_WIFI_APP or raises the existing
%      singleton*.
%
%      H = SML_WIFI_APP returns the handle to a new SML_WIFI_APP or the handle to
%      the existing singleton*.
%
%      SML_WIFI_APP('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in SML_WIFI_APP.M with the given input arguments.
%
%      SML_WIFI_APP('Property','Value',...) creates a new SML_WIFI_APP or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before SML_wifi_app_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to SML_wifi_app_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help SML_wifi_app

% Last Modified by GUIDE v2.5 01-Oct-2018 00:48:15

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @SML_wifi_app_OpeningFcn, ...
                   'gui_OutputFcn',  @SML_wifi_app_OutputFcn, ...
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


% --- Executes just before SML_wifi_app is made visible.
function SML_wifi_app_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to SML_wifi_app (see VARARGIN)

% Choose default command line output for SML_wifi_app
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);
% UIWAIT makes SML_wifi_app wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = SML_wifi_app_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in togglebutton1.
function togglebutton1_Callback(hObject, eventdata, handles)
Temp = [];
press = [];
%arr = [];
%counterI = 1;
while get(hObject,'Value')
    options = weboptions('Timeout' , 60);
    strTemp = webread('http://192.168.4.1/temp' , options)
    floatTemp = str2num(strTemp)
     strPress = webread('http://192.168.4.1/press' , options);
     floatPress = str2num(strPress);
    if length(Temp)<10
        Temp = [Temp floatTemp]
    else
        Temp = [Temp(2:end) floatTemp]
    end
    
     if length(press)<10
         press = [press floatPress];
     else
         press = [press(2:end) floatPress];
     end
 %counterI = counterI + 1;
   axes(handles.axes1)
   plot (Temp);
   axes(handles.axes2)
   plot(press);
   pause(0.1);
end

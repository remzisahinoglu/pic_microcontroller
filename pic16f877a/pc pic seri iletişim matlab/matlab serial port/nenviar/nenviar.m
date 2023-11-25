function varargout = nenviar(varargin)
% NENVIAR M-file for nenviar.fig
%      NENVIAR, by itself, creates a new NENVIAR or raises the existing
%      singleton*.
%
%      H = NENVIAR returns the handle to a new NENVIAR or the handle to
%      the existing singleton*.
%
%      NENVIAR('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in NENVIAR.M with the given input arguments.
%
%      NENVIAR('Property','Value',...) creates a new NENVIAR or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before nenviar_OpeningFunction gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to nenviar_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help nenviar

% Last Modified by GUIDE v2.5 23-Oct-2003 09:21:14

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @nenviar_OpeningFcn, ...
                   'gui_OutputFcn',  @nenviar_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin & isstr(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before nenviar is made visible.
function nenviar_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to nenviar (see VARARGIN)

% Choose default command line output for nenviar
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes nenviar wait for user response (see UIRESUME)
% uiwait(handles.figure1);


function valor_enviado_Callback(hObject, eventdata, handles)
% hObject    handle to valor (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of valor as text
%        str2double(get(hObject,'String')) returns contents of valor as a double


% --- Outputs from this function are returned to the command line.
function varargout = nenviar_OutputFcn(hObject, eventdata, handles)
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes during object creation, after setting all properties.
function valor_enviado_CreateFcn(hObject, eventdata, handles)
% hObject    handle to valor_enviado (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc
    set(hObject,'BackgroundColor','white');
else
    set(hObject,'BackgroundColor',get(0,'defaultUicontrolBackgroundColor'));
end


% --- Executes on button press in boton_enviar.
function boton_enviar_Callback(hObject, eventdata, handles)
% hObject    handle to boton_enviar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Definicion de identificadores para los objetos definidos.
nidentificadores;

% Captura del dato ingresado via teclado.
valor=get(hvalor_enviado,'String');

% Control de dato no valido, colocacion en negro de las etiquetas
% y aviso al usuario.    
if str2num(valor)<0 | str2num(valor)>255
    netiquetas;
    set(haviso,'String','VALOR FUERA DE RANGO . . .');
    set(hvalor_enviado,'String','');
    set(hvalor_presentado,'String','');
else
    % Eliminacion del aviso correspondiente al envio anterior.
    set(haviso,'String','');
    
    % Transformacion en dato numerico del dato string ingresado
    % y presentacion del resultado en pantalla.
    Byte=str2num(dec2bin(str2num(valor),8));
    npantalla;
    set(hvalor_presentado,'String',valor);
    set(hvalor_enviado,'String','');

    % Nueva transformacion del dato string a numerico
    % y envio a la tarjeta del dato ingresado.
    valor_tarjeta=str2num(valor);
    matport('outp',888,valor_tarjeta);
end


% --------------------------------------------------------------------
function recibir_Callback(hObject, eventdata, handles)
% hObject    handle to recibir (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcf);
nrecibir;


% --------------------------------------------------------------------
function reenviar_Callback(hObject, eventdata, handles)
% hObject    handle to reenviar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcf);
nreenviar;


% --------------------------------------------------------------------
function cerrar_Callback(hObject, eventdata, handles)
% hObject    handle to cerrar (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
close(gcf);
npp;


%%
%% simreport.cls
%% -------------------------------------------
%% LaTeX Class Template for Simulation Report
%%
%% Author: Wuqiong Zhao
%% License: MIT
%% Date: 2022/09/05
%% Version: 0.0.1

\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{simreport}[2022/09/05 Simulation Report]
\RequirePackage[T1]{fontenc}

%% Class and Options
\RequirePackage{xkeyval}
\def\@@ptsize{10pt} % font size
\DeclareOptionX{9pt}{\def\@@ptsize{9pt}}
\DeclareOptionX{10pt}{\def\@@ptsize{10pt}}
\DeclareOptionX{11pt}{\def\@@ptsize{11pt}}
\DeclareOptionX{12pt}{\def\@@ptsize{12pt}}
\def\@@papersize{a4paper}
\DeclareOptionX{a4paper}{\def\@@papersize{a4paper}}
\DeclareOptionX{letterpaper}{\def\@@papersize{letterpaper}}
\def\@@mmcesim{0}
\DeclareOptionX{mmcesim}{\def\@@mmcesim{1}} % solution mode
\def\@twoside{0} % default as oneside
\DeclareOptionX{oneside}{\def\@twoside{0}} % one-side document
\DeclareOptionX{twoside}{\def\@twoside{1}} % two-side document
\ProcessOptionsX\relax
\LoadClass[onecolumn,\@@ptsize]{article}

%% Page Settings
\RequirePackage[\@@papersize,inner=2.0cm,outer=2.0cm,top=1.2cm,bottom=3.5cm]{geometry}

%% Document Propertities
\global\let\@date\today
\global\let\@rtime\@empty
\global\let\@simapp\@empty
\global\let\@version\@empty
\global\let\@logo\@empty
\newcommand{\simapp}[1]{\gdef\@simapp{#1}} % simulation app name
\newcommand{\rtime}[1]{\gdef\@rtime{#1}} % report generation time
\newcommand{\version}[1]{\gdef\@version{#1}} % simulation app version
\newcommand{\logo}[1]{\gdef\@logo{#1}} % simulation app logo

\if\@@mmcesim1
  \logo{fig/mmCEsim_logo_256.png}
  \simapp{mmCEsim}
  \version{0.1.0}
\fi

% additional height for footer on the first page
\newcommand{\firstfooteradditionalheight}{\ifx\@logo\@empty30pt\else60pt\fi}
%% Captions Settings
\RequirePackage[font=footnotesize,labelfont=bf]{caption}

%% Header and Footer
\RequirePackage{fancyhdr}
\setlength{\headheight}{52pt}
\setlength{\marginparwidth}{2cm}
\pagestyle{fancy}
\if\@twoside0
  \lhead{\textsc{\@title} -- \@author}
  \rhead{\textsf{\@simapp}}
\else
  \fancyhead[LO]{\textsc{\@title} -- \@author}
  \fancyhead[RE]{\textsf{\@simapp}}
\fi

\RequirePackage{multirow}
\fancypagestyle{firstpage}{
  \renewcommand\headrule{}
  \lhead{}
  \rhead{}
  \cfoot{%
    \vspace*{-\firstfooteradditionalheight}%
    \hrule%
    \begin{center}
      \ifx\@logo\@empty
        Simulated by \@simapp.
        Report generated at \@date\ifx\@rtime\@empty\else, \@rtime\fi.
      \else
        \vspace*{-2.5em}
        \renewcommand{\arraystretch}{2}
        \begin{tabular}{ll@{\hspace{12pt}}|l}
          ~\\
          \multirow{2}{*}{\includegraphics[height=5em]{\@logo}} & Simulated by
          & v\@version \\
          & {\fontsize{20.74}{28}\selectfont\textsf{\@simapp}} &
          Report generated at \@date\ifx\@rtime\@empty\else, \@rtime\fi.
        \end{tabular}
      \fi
    \end{center}
  }
}

\RequirePackage{graphicx}
\let\@@author\@author
\let\@@title\@title
\renewcommand{\@maketitle}{%
  \begin{center}
    {\huge\textbf{\@title}}\\[1\baselineskip]

    {\large\@author}\vspace*{2\baselineskip}
  \end{center}
}
\renewcommand{\maketitle}{\par
  \begingroup
  \newpage
  \global\@topnum\z@ % Prevents figures from going at top of page.
  \@maketitle
  \endgroup
  \thispagestyle{firstpage}
  \enlargethispage{-\firstfooteradditionalheight} % make room for the footer
}

%% Code Block Settings
\RequirePackage[usenames,dvipsnames]{xcolor}
\RequirePackage{listings}
\definecolor{simtools@code@dkgreen}{rgb}{0,0.5,0}
\definecolor{simtools@code@gray}{rgb}{0.5,0.5,0.5}
\definecolor{simtools@code@mauve}{rgb}{0.58,0,0.82}
\lstset{
  numbers=left,  
  frame=tb,
  aboveskip=3mm,
  belowskip=3mm,
  showstringspaces=false,
  columns=fixed,
  framerule=1pt,
  rulecolor=\color{gray!35},
  backgroundcolor=\color{gray!5},
  basicstyle={\ttfamily\small},
  numberstyle=\footnotesize\color{gray},
  keywordstyle=[1]\bfseries\color{MidnightBlue!95!black},
  keywordstyle=[2]\bfseries\color{Orange!70!blue},
  keywordstyle=[3]\bfseries\color{simtools@code@mauve},
  keywordstyle=[4]\bfseries\color{red},
  keywordstyle=[5]\bfseries\color{Brown},
  keywordstyle=[6]\bfseries\color{blue},
  commentstyle=\color{simtools@code@dkgreen},
  stringstyle=\color{simtools@code@mauve},
  breaklines=true,
  breakatwhitespace=true,
  tabsize=2,
  extendedchars=false,
  postbreak=\mbox{\hspace{-1.4em}\textcolor{purple}{$\hookrightarrow$}\space}
}

\lstdefinelanguage{mmcesim-sim}
{
  alsoletter={:\\.`*},
  keywords=[1]{BRANCH,BREAK,CALC,CALL,COMMENT,CPP,ESTIMATE,INIT,MATLAB,MERGE,NEW,OCTAVE,PRINT,PYTHON,RECOVER,ELSE,ELIF,FOR,FOREVER,FUNCTION,IF,LOOP,WHILE,END,ELSE,ELIF},
  keywords=[2]{\\abs,\\find,\\kron,\\reshape,\\size,\\zeros,\\ones,\\length,\\index_max,\\index_min,\\max,\\min,\\ismember,\\pinv,\\inv,\\sum,\\accu,\\set_size,\\range,\\sort_index,\\solve,\\pow},
  keywords=[3]{`ALG_NAME`,`ALG_PARAMS`,`ALG_NUM`,`BEAM.R.x`,`BEAM.R.y`,`BEAM.R`,`BEAM.T.x`,`BEAM.T.y`,`BEAM.T`,`BEAM.*`,`DICTIONARY.R`,`DICTIONARY.T`,`GRID.R.x`,`GRID.R.y`,`GRID.R`,`GRID.T.x`,`GRID.T.y`,`GRID.T`,`GRID.*`,`JOB_NUM`,`JOB_CNT`,`MEASUREMENT`,`NMSE`,`PILOT`,`SIZE.R.x`,`SIZE.R.y`,`SIZE.R`,`SIZE.T.x`,`SIZE.T.y`,`SIZE.T`,`SIZE.*`,`SNR_dB`,`SNR_LINEAR`,`VERSION`,`OFDM_ANGLE_EST_NUM`,`SPARSITY_EST`,`CARRIERS_NUM`,`OFDM_RE_ESTIMATE`},
  keywords=[6]{version:,meta:,title:,description:,author:,email:,website:,license:,date:,comments:,physics:,frequency:,off_grid:,nodes:,id:,role:,num:,size:,beam:,grid:,beamforming:,variable:,scheme:,channels:,from:,to:,sparsity:,gains:,mode:,mean:,variance:,sounding:,variables:,received:,noise:,channel:,preamble:,estimation:,conclusion:,simulation:,backend:,metric:,jobs:,name:,test_num:,SNR:,SNR_mode:,pilot:,algorithms:,alg:,max_iter:,label:,estimated_channel:,report:,format:,plot:,table:,latex:,command:,UTF8:,appendix:,stopping_thr:,params:,macro:,value:,carriers:,in_alg:},
  upquote,
  % morestring=[b]`,
  morecomment=[l]{\#},
  otherkeywords={::,[,],^,@,\$},
  morekeywords=[4]{::},
  morekeywords=[5]{^,[,],@,\$}
}

\RequirePackage{pgfplotstable}
\pgfplotsset{compat=newest}
\RequirePackage{booktabs}
\pgfplotstableset{
  every head row/.style={before row=\toprule, after row=\midrule},
  every last row/.style={after row=\bottomrule},
  fixed, fixed zerofill, precision=2,
  column type=c,
  columns/Algorithm/.style={string type},
  display columns/0/.style={fixed=false,fixed zerofill=false}
}

\newcommand{\simtools@detokenize}{}% for safety
\DeclareRobustCommand{\simtools@detokenize}[1]{\detokenize{#1}}

\definecolor{simtools@blued}{RGB}{0,114,189}
\definecolor{simtools@red}{RGB}{217,83,25}
\definecolor{simtools@green}{RGB}{32,134,48}
\definecolor{simtools@yellow}{RGB}{237,137,32}
\definecolor{simtools@purple}{RGB}{126,47,142}
\definecolor{simtools@blues}{RGB}{77,190,238}
\pgfplotscreateplotcyclelist{simtools}{
  {simtools@blued, thick, mark=o, mark size=2},
  {simtools@red, thick, mark=diamond, mark size=2.8},
  {simtools@green, thick, mark=square, mark size=2},
  {simtools@yellow, thick, mark=triangle, mark size=3},
  {simtools@purple, thick, mark=asterisk, mark size=3},
  {simtools@blues, thick, mark=pentagon, mark size=3},
  {gray, thick, mark=+, mark size=3}
}

\newcommand{\simtable}[1]{%
  \pgfplotstabletypeset{#1}%
}

\newcommand{\simplot}[1]{%
  \begin{tikzpicture}%
    \pgfplotstableread{#1}{\table}%
    \pgfplotstablegetcolsof{#1}%
    \pgfmathtruncatemacro\numberofcols{\pgfplotsretval-1}%
    \pgfplotstablegetrowsof{#1}%
    \pgfmathtruncatemacro\numberofrows{\pgfplotsretval-1}%
    \ifnum\numberofrows>2% only plot the table if data has more than 1 row
      \pgfplotstablegetcolumnnamebyindex{0}\of{\table}\to{\colzeroname}%
      \begin{axis}[%
        width=.65\linewidth,%
        legend pos=outer north east,%
        enlargelimits=false,%
        xlabel=\colzeroname,%
        ylabel={NMSE [dB]},%
        cycle list name=simtools,%
        grid=both,%
        ticklabel style={/pgf/number format/.cd, fixed, fixed zerofill=false}%
      ]%
        \pgfplotsinvokeforeach{1,...,\numberofcols}{%
          \pgfplotstablegetcolumnnamebyindex{##1}\of{\table}\to{\colname}%
          \addplot table [y index=##1] {#1};%
          \addlegendentryexpanded{\colname}%
        }%
      \end{axis}%
    \fi%
  \end{tikzpicture}%
}

\newcommand\blfootnote[1]{%
  \begingroup
  \renewcommand\thefootnote{}\footnote{#1}%
  \addtocounter{footnote}{-1}%
  \endgroup
}

\RequirePackage{etoolbox}
\newcommand{\simjob}[3]{%
  \clearpage
  \ifnumequal{\the\value{section}}{1}{\section{Simulation Results}}{}
  \subsection{\simtools@detokenize{#1}}
  \blfootnote{Simulated with #3 Monte Carlo tests.}%
  \simtable{#2}

  \vspace*{1\baselineskip}
  \simplot{#2}
}

\newcommand{\simcode}[1]{%
  \ifnumequal{\the\value{section}}{2}{\clearpage\section{Simulation Configuration}}{}
  \ifnumequal{\the\value{subsection}}{0}{\subsection{Configuration File}}{}
  \lstinputlisting[language=mmcesim-sim,caption={\ttfamily\protect\filename@parse{\simtools@detokenize{#1}}\protect\filename@base.sim}]{../#1.sim}
}
\newcommand{\algcode}[1]{%
  \ifnumequal{\the\value{subsection}}{1}{\subsection{Algorithms}}{}
  \lstinputlisting[language=mmcesim-sim,caption={\ttfamily\protect\filename@parse{\simtools@detokenize{#1}}\protect\filename@base.alg}]{#1.mmcesim-alg}
}

\newcommand{\simsystem}[9]{%
  \def\ST@tempa{#1}
  \def\ST@tempb{#2}
  \def\ST@tempc{#3}
  \def\ST@tempd{#4}
  \def\ST@tempe{#5}
  \def\ST@tempf{#6}
  \def\ST@tempg{#7}
  \def\ST@temph{#8}
  \def\ST@tempi{#9}
  \simsystem@continued
}
\newcommand{\simsystem@continued}[7]{%
  \tableofcontents
  \section{System Settings}
  The simulation adopts the geometric channel model for millimeter wave (mmWave).
  
  \vspace*{1\baselineskip}
  \begin{tabular}{cccc}%
    \toprule%
    Name & Antenna Number & Beam Number & Grid Number \\%
    \midrule%
    Transmitter & $\ST@tempa\times\ST@tempb$ & $\ST@tempc\times\ST@tempd$ & $\ST@tempe\times\ST@tempf$ \\%
    Receiver & $\ST@tempg\times\ST@temph$ & $\ST@tempi\times#1$ & $#2\times#3$ \\%
    \bottomrule%
  \end{tabular}%

  \begin{itemize}
    \item Channel Sparsity: $#4$;
    \item Off Grid Effect: #5;
    \ifx1#6%
      \item Bandwidth: Wideband;
      \item Carriers: #7.
    \else
      \item Bandwidth: Narrowband;
    \fi%
  \end{itemize}
}

\if\@@mmcesim1
  \AtEndDocument{%
    \section{mmCEsim Information}
    This report is auto generated by mmCEsim.
    The application \textbf{mmCEsim} is a powerful tool to simulate millimeter wave (mmWave) channel estimation (CE)
    for both experts and learners.
    
    mmCEsim is \textit{open source}!
    The software can be freely used and distributed under the MIT license.
    \begin{itemize}
      \item Official Website: \url{https://mmcesim.org}
      \item Documentation: \url{https://mmcesim.org/doc}
      \item Tutorial: \url{https://mmcesim.org/tutorial}
      \item Examples: \url{https://mmcesim.org/example}
      \item Web Application: \url{https://app.mmcesim.org}
      \item Blog: \url{https://blog.mmcesim.org}
      \item Publications: \url{https://pub.mmcesim.org}
      \item GitHub Organization: \url{https://github.com/mmcesim}
      \item Twitter: \url{https://twitter.com/mmcesim}
      \item VS Code Extension: \url{https://marketplace.visualstudio.com/items?itemName=mmcesim.mmcesim}
    \end{itemize}
  }
\fi

\RequirePackage[pdfusetitle,hidelinks]{hyperref}
\hypersetup{
  pdfcreator={\@simapp},
  pdfsubject={Simulation},
  pdfkeywords={%
    \if\@@mmcesim1%
      mmCEsim, Millimeter Wave, mmWave, Channel Estimation, Simulation, Report
    \else%
      Simulation, Report
    \fi%
  }
}
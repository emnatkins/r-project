                            >  '     e  Z   	  
�    #   File:       nlsLib.make#   Target:     nlsLib#   Created:    Jago, Jul 2001, Stefano M. Iacus##   This script assumes that R binary already exists in #   folder 'src/macintosh/bin'##   Tested with:##   MPW Shell 3.6d7#   MrC C Compiler 5.0.0d3c1#   Universal Headers 3.4#   CarbonLib 1.4#DLib			= nlsMAKEFILE        = {DLib}Lib.make�MondoBuild�    = {MAKEFILE}  # Make blank to avoid rebuilds when makefile is modifiedIncludes        = -i ::::include: �				  -i ::::macintosh: �Sym-PPC         = -sym offPPCCOptions     = {Includes} {Sym-PPC} -opt off -includes unix -w 35,2 -shared_lib_export on -d HAVE_CONFIG_H -d Macintosh �					-d TARGET_API_MAC_CARBON=1 -prefix RHeaders.h -align power					### Library directory ###LibDir     		= ::::macintosh:bin:library:{DLib}:libs### Source Files ###SrcFiles        =  nls.c### Object Files ###ObjFiles-PPC    =  nls.o### Libraries ###LibFiles-PPC    =  �				  "{SharedLibraries}CarbonLib" �#				  "{SharedLibraries}StdCLib" �				  "::::macintosh:bin:R"  �				  "{PPCLibraries}PPCCRuntime.o" ### Default Rules ### .o  �  .c  {�MondoBuild�}	{PPCC} {depDir}{default}.c -o {targDir}{default}.o {PPCCOptions}### Build Rules ###{DLib}Lib  ��  {ObjFiles-PPC} {LibFiles-PPC} {�MondoBuild�} #create export table    if `Exists :expvar`	 delete :expvar	end    catenate �.x > expvar# checks if modules directory exsists    if ! `Exists -d "{LibDir}"`	 echo "Creating libs directory" "{LibDir}"      NewFolder "{LibDir}"    end# Build the vfonts module		PPCLink �		-o {LibDir}:{Dlib}Lib �		{ObjFiles-PPC} �		{LibFiles-PPC} �		{Sym-PPC} �		-mf -d �		-t 'shlb' �		-c '????' �		-xm s �		-@export expvar     # Removing mass    Delete   �.x	Delete   �.o     Delete :expvar	### Required Dependencies ###nls.o  �  nls.c### Optional Dependencies ###### Build this target to generate "include file" dependencies. ###Dependencies  �  $OutOfDate	MakeDepend �		-append {MAKEFILE} �		-ignore "{CIncludes}" �		-objdir ":" �		-objext .o �		{Includes} �		{SrcFiles}     �   �   m                                                                                                                                                                                                                                                   J 	Monaco    6�@                     < $�- < $�-���%  7  7   ��   $   �  �	lapackLib  �  �StdCLib    < $�- < $�-               Q�q�?�20         ������%                iacus   nlsLib.make    mpw makefile      �   �   mSORT� �  ^ MPSR  ckid   6���        ���   N    ���   v     �     ����Projector DataTEXTMPS  ����                  
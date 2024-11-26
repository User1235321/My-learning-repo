set expandtab
set smarttab
set tabstop=2
set softtabstop=2
set shiftwidth=2
set number
set foldcolumn=2
set autoindent
set smartindent
set t_Co=256
set termguicolors
set background=dark
colorscheme pablo
:au BufWinEnter * let w:m1=matchadd('Search', '\%<121v.\%>117v', -1)
:au BufWinEnter * let w:m2=matchadd('ErrorMsg', '\%>120v.\+', -1)
au BufNewFile,BufRead * let b:mtrailingws=matchadd('ErrorMsg', '\s\+$', -1)
au BufNewFile,BufRead * let b:mtabbeforesp=matchadd('ErrorMsg', '\v(\t+)\ze( +)', -1)
au BufNewFile,BufRead * let b:mtabaftersp=matchadd('ErrorMsg', '\v( +)\zs(\t+)', -1)
syntax on

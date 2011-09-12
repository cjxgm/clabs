syntax enable
syntax on
colorscheme delek

set number
set autoindent
set smartindent
set tabstop=4
set shiftwidth=4
set incsearch
set mouse=a
"set foldmethod=indent
"set textwidth=70

"map <F2> zA
"map <F3> zM
map <F2> "+y
map <F3> "+p
map <F5> :wa<CR>:make debug<CR>
map <F6> :wa<CR>
map <F9> :wa<CR>:!./configure<CR>
map <F12> mtgg=G't

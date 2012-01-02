(defun 制作cd (名 艺术家 投票 已复制)
  (list :名 名 :艺术家 艺术家 :投票 投票 :已复制 已复制))

(defvar *数据库* nil)

(defun 添加记录 (记录) (push 记录 *数据库*))

(defun 添加cd (名 艺术家 投票 已复制)
  (添加记录 (制作cd 名 艺术家 投票 已复制)))

(defun 打印数据库 ()
;  (dolist (cd *数据库*)
;    (format t "~{~a:~10t~a~%~}~%" cd)))
   (format t "~{~{~a:~10t~a~%~}~%~}" *数据库*))

(defun 请求输入 (提示符)
  (format *query-io* "~a: " 提示符)
  (force-output *query-io*)
  (read-line *query-io*))

(defun 请求输入一条记录 ()
  (添加cd
	(请求输入 "名")
	(请求输入 "艺术家")
	(or (parse-integer (请求输入 "投票") :junk-allowed t) 0)
	(y-or-n-p "已复制")))

(defun 请求输入记录 ()
  (loop
	(请求输入一条记录)
	(if (not (y-or-n-p "再添加一个？")) (return))))

(defun 保存数据库 (文件名)
  (with-open-file (输出 文件名
					   :direction :output
					   :if-exists :supersede)
	(with-standard-io-syntax
	  (print *数据库* 输出))))


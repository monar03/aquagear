<?php
namespace aquagear\handler;

/**
 * PHPエラーのハンドリングクラス.<br />
 * PHPのエラーが出たときに呼ばれる.
 * 
 * @package aquagear 
 * @subpackage handler
 * 
 * @author motonari
 * @version $Id: PhpErrorHandler.inc 535 2011-07-26 14:02:27Z motonari $
 */
class PhpErrorHandler extends CoreHandler {
	/**
	 * ハンドラの初期化.
	 * 
	 * @return void
	 */
	public static function initHandler() {
		@set_error_handler(array('\aquagear\handler\PhpErrorHandler', 'errorHandler'));
	}
	
	/**
	 * PHPエラーハンドラ.
	 * 
	 * @param int    $errno エラーレベル
	 * @param string $errstr エラーメッセージ
	 * @param string $errfile エラーファイル名
	 * @param int    $errline エラー行数
	 * @param array  $errcontext 格納したすべての変数
	 * @return mixed
	 * 
	 * @see http://www.php.net/set_error_handler
	 * 
	 */
	public static function errorHandler($errno , $errstr, $errfile, $errline, $errcontext) {
		switch($errno) {
			case E_NOTICE:
			case E_USER_NOTICE:
				\aquagear\log\Logger::getLogInstance()->debug("[PHP-NOTICE] $errstr [$errfile : $errline]");
				break;
			case E_WARNING:
			case E_CORE_WARNING:
			case E_COMPILE_WARNING:
			case E_USER_WARNING:
				\aquagear\log\Logger::getLogInstance()->warning("[PHP-WARNING] $errstr [$errfile : $errline]");
				break;
			case E_PARSE:
				\aquagear\log\Logger::getLogInstance()->error("[PHP-PARSE] $errstr [$errfile : $errline]");
				throw new \aquagear\exception\PhpErrorException("[PHP-PARSE] $errstr [$errfile : $errline]", '500');
				break;
			case E_ERROR:
			case E_CORE_ERROR:
			case E_COMPILE_ERROR:
			case E_USER_ERROR:
			case E_RECOVERABLE_ERROR:
				$dump_filepath = md5(microtime());
				\aquagear\file\FileReaderWriter::write($dump_filepath, var_export($errcontext, true), LOCK_EX, false);
				\aquagear\log\Logger::getLogInstance()->error("[PHP-ERROR] $errstr [$errfile : $errline] [dump file : $dump_filepath]");
				throw new \aquagear\exception\PhpErrorException("[PHP-ERROR] $errstr [$errfile : $errline] [dump file : $dump_filepath]", '500');
				break;
			case E_STRICT:
				\aquagear\log\Logger::getLogInstance()->debug("[PHP-STRICT] $errstr [$errfile : $errline]");
				break;
			default:
				\aquagear\log\Logger::getLogInstance()->debug("[PHP-OTHER] $errstr [$errfile : $errline]");
				break;
		}
	}
}
?>
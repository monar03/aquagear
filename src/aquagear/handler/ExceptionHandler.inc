<?php 
namespace aquagear\handler;

/**
 * 例外処理のハンドラ. <br />
 * 例外が発生した場合は、この例外ハンドラにより、キャッチされる.
 * 
 * @package aquagear 
 * @subpackage handler
 * 
 * @author motonari
 * @version $Id: ExceptionHandler.inc 535 2011-07-26 14:02:27Z motonari $
 */
class ExceptionHandler extends CoreHandler {
	/**
	 * (non-PHPdoc)
	 * @see aquagear/handler/\aquagear\handler\CoreHandler#initHandler()
	 */
	public static function initHandler() {
		@set_exception_handler(array('\aquagear\handler\ExceptionHandler',  'exceptionHandler'));
	}
	
	/**
	 * 例外の発生時に、このメソッドが呼ばれる.
	 * 
	 * @param object $exception 発生した例外
	 * @return void
	 * 
	 * @see http://www.php.net/manual/ja/function.set-exception-handler.php
	 */
	public static function exceptionHandler($exception) {
		\aquagear\log\Logger::getLogInstance()->warning('exception ['.get_class($exception).'] ['. $exception->getMessage() . "]\n" . $exception->getTraceAsString());
		
		if($exception instanceof \aquagear\exception\CoreException) {
			$exception->error();
		}
		else {
			\aquagear\utility\HttpUtility::errorRedirect();
		}
	}
}
?>
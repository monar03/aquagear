<?php
namespace aquagear\exception;

/**
 * 例外処理のコアクラス.
 * 
 * @package aquagear
 * @subpackage exception
 * 
 * @author motonari
 * @version $Id: CoreException.inc 535 2011-07-26 14:02:27Z motonari $
 */ 
abstract class CoreException extends \Exception {
	/**
	 * 例外処理がとんだ場合、このメソッドが呼ばれる.
	 * 
	 * @return void
	 */
	public function error() {
		\aquagear\error\ErrorDisplay::errorDisplay($this->getCode());
	}
}
?>
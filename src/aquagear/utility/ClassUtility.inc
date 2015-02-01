<?php 
namespace aquagear\utility;

/**
 * クラス関係のutility
 * 
 * @package aquagear
 * @subpackage utility
 * 
 * @author motonari
 * @version $Id: ClassUtility.inc 534 2011-07-26 13:48:33Z motonari $
 *
 */
class ClassUtility extends \aquagear\Object {
	/**
	 * コンストラクタ.
	 * 
	 * @return void
	 */
	private function __construct() {
		
	}
	
	/**
	 * メソッドを呼ぶためのmethod.
	 * 
	 * @param mixed $class_mixed クラス名 or クラスオブジェクト
	 * @param string $method_str メソッド名
	 * @param array $args 引数
	 * @return mixed 実行結果
	 */
	public static function callMethod($class_mixed, $method_str, array $args=array()) {
		if(empty($args)) {	
			$result = call_user_func(array($class_mixed, $method_str));
		}
		else {
			$result = call_user_func_array(array($class_mixed, $method_str), $args);
		}
		
		return $result;
	}
}
?>
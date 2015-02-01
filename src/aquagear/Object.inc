<?php
namespace aquagear;

/** 
 * すべての基底のobjectクラス
 * 
 * @package aquagear
 * 
 * @author motonari
 * @version $Id: Object.inc 537 2011-07-27 14:36:38Z motonari $
 */
abstract class Object {
	/**
	 * setter
	 * 
	 * @param string $arg 引数
	 * @param string $value 代入値
	 * @return void
	 */
	public function __set($arg, $value) {
		throw new \Exception('can not set value.', '500');
	}
	
	/**
	 * getter
	 * 
	 * @param string $arg 引数
	 * @return void
	 */
	public function __get($arg) {
		throw new \Exception('can not get value.', '500');
	}
	
	/**
	 * call function
	 * 
	 * @param string $name 関数名
	 * @param array $arguments 引数
	 * @return void
	 */
	public function __call($name, $arguments) {
		throw new \Exception("can not call function", '500');
	}
	
	/**
	 * static call function
	 * 
	 * @param string $name 関数名
	 * @param array $arguments 引数
	 * @return void
	 */
	public static function __callStatic($name, $arguments) {
		throw new \Exception("can not call static function", '500');
	}
	
	/**
	 * toString
	 * 
	 * @return string オブジェクト名
	 */
	public function __toString() {
		return get_class($this);
	}
	
}
?>
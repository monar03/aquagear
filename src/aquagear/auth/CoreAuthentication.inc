<?php
namespace aquagear\auth;
 
/**
 * 認証のロジッククラス.
 * 
 * @package aquagear
 * @subpackage auth
 * 
 * @author motonari
 * @version $Id$
 *
 */
abstract class CoreAuthentication extends \aquagear\Object {
	
	/**
	 * 初期化メソッド.
	 * 
	 * @param obj $conf_obj 設定ファイルオブジェクト
	 * @return bool スタックにのせる場合true
	 */
	abstract public function init($conf_obj=null);
	
	/**
	 * 実行メソッド.
	 * 
	 * @return array 認証結果、認証失敗時はfalse
	 */
	abstract public function execute();
}
?>
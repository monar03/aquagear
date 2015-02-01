<?php 
namespace aquagear\control;

/**
 * コントローラーの基底クラス.
 * 
 * @package aquagear
 * @subpackage control
 * 
 * @author motonari
 * @version $Id$
 *
 */
abstract class CoreWebApiControl extends \aquagear\Object {
	/**
	* 初期化メソッド.
	*
	* @param object $conf_obj 設定ファイルオブジェクト.
	* @return void
	*/
	abstract public function init($conf_obj=null);
		
	/**
	 * 実行メソッド.
	 * 
	 * @return array APIを作成するデータ
	 */
	abstract public function execute();
}
?>
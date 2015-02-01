<?php 
namespace aquagear\command;

/**
 * コマンドの基底クラス.
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id: CoreCommand.inc 536 2011-07-26 14:58:17Z motonari $
 */
abstract class CoreCommand extends \aquagear\Object {

	/**
	 * コンストラクタ
	 * 
	 * @return void
	 */
	public function __construct() {
	
	}
	
	/**
	 * 初期化メソッド.
	 * 
	 * @return bool.
	 */
	abstract public function init();
	
	/**
	 * 実行メソッド.
	 * 
	 * @return int 実行完了引数.
	 */
	abstract public function execute();
	
	/**
	 * オブジェクトの取得.
	 * 
	 * @return object
	 */
	public static function getInstance(){ return null; }
}
?>
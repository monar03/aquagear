<?php 
namespace aquagear\control;

/**
 * コントローラーの基底クラス.
 * 
 * @package aquagear
 * @subpackage control
 * 
 * @author motonari
 * @version $Id: CoreControl.inc 536 2011-07-26 14:58:17Z motonari $
 *
 */
abstract class CoreControl extends \aquagear\Object {
	/**
	 * テンプレートパス.
	 * 
	 * @var string テンプレートパス
	 */
	protected $_template_path = "";
	
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
	 * @return void
	 */
	abstract public function execute();
	
	/**
	 * テンプレートのパスのセット.
	 * 
	 * @param string $template_path テンプレートのパス
	 * @return void
	 */
	protected final function setTemplate($template_path) {
		if(is_file($template_path)) {
			$this->_template_path = $template_path;
		}
	}
	
	/**
	 * テンプレートパスの取得.
	 * 
	 * @return string
	 */
	public final function getTemplate() {
		return $this->_template_path;
	}
}
?>
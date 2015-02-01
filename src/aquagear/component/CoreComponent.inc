<?php 
namespace aquagear\component;

/**
 * コンポーネントの基底クラス.
 * 
 * @package aquagear
 * @subpackage component
 * 
 * @author motonari
 * @version $Id: CoreComponent.inc 536 2011-07-26 14:58:17Z motonari $
 *
 */
abstract class CoreComponent extends \aquagear\Object {
	/**
	 * コンポーネントID
	 * 
	 * @var string
	 */
	protected $id = '';
	
	/**
	 * データを保存
	 * 
	 * @var array
	 */
	protected $data_holder = array();
	
	/**
	 * テンプレートベースディレクトリ
	 * 
	 * @var string テンプレートディレクトリ
	 */
	private $_template_base_dir = "";
	
	/**
	 * テンプレートパス.
	 * 
	 * @var string テンプレートパス
	 */
	private $_template_path = "";
	
	/**
	 * 設定ファイル配列.
	 * 
	 * @var array
	 */
	protected $component_arr = array();
	
	/**
	 * キャッシュの設定.
	 * 
	 * @var array
	 */
	private $_cache = array();
	
	/**
	 * 初期化メソッド.
	 * 
	 * @param string $id ID
	 * @param array $cache キャッシュ設定
	 * @param string $template_path テンプレートパス
	 * @param array $component_arr 設定ファイルオブジェクト.
	 * 
	 * @return void
	 */
	public final function init($id, array $cache, $template_base_dir, $template_path, array $component_arr=array()) {
		$this->id             = $id;
		$this->_cache         = $cache;
		$this->component_arr  = $component_arr;
		$this->_template_base_dir = $template_base_dir;
		$this->_template_path = $template_path;
	}
	
	/**
	 * テンプレートルートの設定
	 * 
	 * @param string $template_root テンプレートルート
	 * 
	 * @return void
	 */
	public final function setTemplateRoot($template_root) {
		$this->_template_base_dir = $template_root;
	}
	
	/**
	 * 実行メソッド.
	 * 
	 * @return void
	 */
	public function execute() {
		if($this->_cache['set'] === 'on' &&
			\aquagear\cache\Cache::getCacheInstance($this->_cache['type'])->cacheExists(md5($this->id . serialize($this->component_arr)), $this->_cache['ttl'])) {
			return \aquagear\cache\Cache::getCacheInstance($this->_cache['type'])->getValue(md5($this->id . serialize($this->component_arr)));
		}
		else {
			$this->logic();
			$content = $this->_view();
			if($this->_cache['set'] === 'on') {
				\aquagear\cache\Cache::getCacheInstance($this->_cache['type'])->setValue(md5($this->id . serialize($this->component_arr)), $content);
			}
			return $content;
		}
	}
	
	/**
	 * コンポーネントのロジック部分.
	 * 
	 * @return bool 成功の場合true
	 */
	abstract protected function logic();
	
	/**
	 * コンポーネントのview部分.
	 * 
	 * @return string 表示用HTML
	 */
	private function _view() {
		$value = array();
		\aquagear\log\Logger::getLogInstance()->info('read template [' . $this->_template_base_dir . '/' . $this->_template_path . ']');
		if(! is_file($this->_template_base_dir . '/' . $this->_template_path)) {
			return '';
		}
		
		if(! empty($this->data_holder)) {
			if(is_array($this->data_holder)) {
				$value += $this->data_holder;
			}
			
			if(is_array($this->component_arr)) {
				$value += $this->component_arr;
			}
			
			extract($value);
		}
		
		ob_start();
		ob_implicit_flush(0);
		include($this->_template_base_dir . '/' . $this->_template_path);
		return ob_get_clean();
	}
	
	/**
	 * テンプレートのパスのセット.
	 * 
	 * @param string $template_path テンプレートのパス
	 * @return void
	 */
	protected final function setTemplate($template_path) {
		if(is_file($this->_template_base_dir . '/' . $template_path)) {
			$this->_template_path = $template_path;
		}
	}
}
?>
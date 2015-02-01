<?php 
namespace aquagear\command;

/**
 * ビューのコマンド.
 * 
 * @package aquagear
 * @subpackage command
 * 
 * @author motonari
 * @version $Id: MVCCommand.inc 538 2011-07-31 12:52:00Z motonari $
 * 
 */
class MVCCommand extends CoreCommand {
	
	/**
	 * 設定ファイルオブジェクト
	 * 
	 * @var obj
	 */
	private $_conf_obj = null;
	
	/**
	 * テンプレートパス.
	 * 
	 * @var string
	 */
	private $_template_path = null;
	
	/**
	 * キャッシュをするかどうか.
	 * 
	 * @var bool
	 */
	private $_cache_info = false;
	
	/**
	 * ヘッダー用の配列
	 * 
	 * @var array
	 */
	private $_header = false;
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#init()
	 */
	public function init() {
		$conf_path = \aquagear\application\ApplicationData::getApplicationConfDir() .'/mvc.ini'; 
		if(is_file($conf_path)) {
			$this->_conf_obj = \aquagear\holder\ConfigureHolder::load($conf_path);
			
			$view_conf = $this->_conf_obj->getValue('view');
			\aquagear\log\Logger::getLogInstance()->debug("view conf = " . var_export($view_conf, true));
			if(! empty($view_conf) &&
					isset($view_conf['template_path'])) {
				$this->_template_path = $view_conf['template_path'];
			}
			
			$this->_cache_info = $this->_conf_obj->getValue('cache');
			$this->_header = $this->_conf_obj->getValue('header');
		}
		
		if(empty($this->_template_path)){
			$this->_template_path = \aquagear\application\ApplicationData::getAquagearServerEnv('template_root') . \aquagear\holder\HttpParameterHolder::getInstance()->getURI() . ".tpl";
		}
		
		\aquagear\log\Logger::getLogInstance()->info("template path = " . $this->_template_path);
		return true;
	}
	
	/**
	 * (non-PHPdoc)
	 * @see aquagear/command/\aquagear\command\CoreCommand#execute()
	 */
	public function execute() {
		$class_name = "";
		
		if($this->cacheExists()) {
			\aquagear\log\Logger::getLogInstance()->debug('cache exists skip. ' . get_class($this));
			return true;
		}
		
		if($this->_conf_obj instanceof \aquagear\holder\ConfigureHolder) {
			$control_conf = $this->_conf_obj->getValue('control');
		}
		
		if(isset($control_conf['class_name'])) {
			$class_name = $control_conf['class_name'];
		}
		else {
			$class_name = $this->getDefaultClassName();
		}
		
		if(! class_exists($class_name)) {
			\aquagear\log\Logger::getLogInstance()->debug('cannot exists class [ $class_name = ' . $class_name . ' ]');
			return false;
		}
		
		\aquagear\log\Logger::getLogInstance()->debug("execute control [class_name = $class_name]");
		
		if($this->checkControl($class_name)) {
			$control_obj = new $class_name;
		}
		if($control_obj instanceof \aquagear\control\CoreControl) {
			$control_obj->init($this->_conf_obj);
			if($control_obj->execute() !== true) {
				return false;
			}
		}
		else {
			throw new \aquagear\exception\CommandException('not ControlObject.', '404');
		}
		
		$template_path = $control_obj->getTemplate();
		if(! empty($template_path)) {
			$this->_template_path = $template_path;
		}

		if(is_file($this->_template_path)) {
			$this->_sendHeader();
			$contents = $this->_getCache();
			if($contents === false) {
				$contents = $this->_getContents($this->_template_path);
				if($this->_cache_info['cache']) {
					$this->_setCache($contents);
				}
			}
			
			$this->_render($contents);
		}
		else {
			\aquagear\log\Logger::getLogInstance()->warning('can not find template file. [template=' . $this->_template_path . ']');
			return false;
		}

		return true;
	}
	
	/**
	 * コントロールかどうかの判定
	 * 
	 * @param string $class_name クラス名
	 * 
	 * @return bool controlの場合true, ほかfalse
	 */
	private function checkControl($class_name) {
		$path = preg_replace('/_/', '\/', $class_name);
		if(is_file(	$path . '/' . \aquagear\application\ApplicationData::getAquagearServerEnv('control_root'))) {
			return true;
		}
		
		return false;
	}
	
	/**
	 * コントローラのデフォルトのクラス名の取得
	 * 
	 * @return string
	 */
	private function getDefaultClassName() {
		$uri_arr = explode('/', ltrim(\aquagear\holder\HttpParameterHolder::getInstance()->getURI(), '\/'));
		$uri_arr[count($uri_arr) - 1] = ucwords($uri_arr[count($uri_arr) - 1]);

		return  join('_', $uri_arr) . "Control";
	}
	
	/**
	 * コンテンツの取得.
	 * 
	 * @param string $template_path テンプレートパス
	 * 
	 * @return string
	 */
	private function _getContents($template_path) {
		$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
		$contents = "";

		extract(\aquagear\holder\DataHolder::getInstance()->getAllData());
		
		\aquagear\log\Logger::getLogInstance()->debug('use template file path [ template_path = ' . $template_path . ' ]');

		ob_start();
		ob_implicit_flush(0);
		include($template_path);
		$contents = ob_get_clean();
		
		if(empty($contents)) {
			throw new \aquagear\exception\CommandException('empty command view contents.', '500');
		}
		
		return $contents;
	}
	
	/**
	 * キャッシュの取得
	 * 
	 * @return string キャッシュを取得したときはキャッシュの文字列,無い場合はfalse
	 */
	private function _getCache() {
		$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
		$cache_file = md5(\aquagear\application\ApplicationData::getURI(true));
		
		if($this->cacheExists()) {
			return \aquagear\cache\Cache::getCacheInstance($this->_cache_info['cache_type'])->getValue($uri . '/' . $cache_file);
		}
		else {
			return false;
		}
	}
	
	/**
	 * キャッシュのセット
	 * 
	 * @param string $content キャッシュする文字列
	 * 
	 * @return bool 成功時true,失敗時false
	 */
	private function _setCache($contents) {
		$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
		$cache_file = md5(\aquagear\application\ApplicationData::getURI(true));
		
		if(empty($this->_cache_info['boost'])) {
			return \aquagear\cache\Cache::getCacheInstance($this->_cache_info['cache_type'])->setValue($uri . '/' . $cache_file, $contents, $this->_cache_info['ttl']);
		}
		else {
			if(! is_dir('./cache/' . dirname(\aquagear\application\ApplicationData::getURI(true)))) {
				\aquagear\file\FileOperator::mkdir('./cache/' . dirname(\aquagear\holder\HttpParameterHolder::getInstance()->getURI()), 0777, true, true);
			}

			return \aquagear\file\FileReaderWriter::write('./cache/' . \aquagear\holder\HttpParameterHolder::getInstance()->getURI() . \aquagear\holder\HttpParameterHolder::getInstance()->getServerValue('QUERY_STRING') . '.html', $contents);
		}
	}
	
	/**
	 * headerの送信
	 * 
	 * @return void
	 */
	private function _sendHeader() {
		if(is_array($this->_header) === true) {
			$header_arr = $this->_header;
			foreach($header_arr as $header) {
				\aquagear\utility\HttpUtility::sendHeader($header);
			}
		}
	}
	
	/**
	 * レンダリングメソッド.
	 * 
	 * @return void
	 */
	private function _render($contents) {
		echo $contents;
	}
	
	/**
	 * キャッシュの存在確認.
	 * 
	 * @return bool
	 */
	public function cacheExists() {
		if(empty($this->_cache_info['cache_type'])) {
			return false;
		}
		
		if($this->_cache_info['cache']) {
			$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
			return \aquagear\cache\Cache::getCacheInstance($this->_cache_info['cache_type'])->cacheExists($uri . "/" . md5(\aquagear\application\ApplicationData::getURI(true)), $this->_cache_info['ttl']);
		}
		
		return false;
	}
	
	/**
	 * キャッシュの削除.
	 * 
	 * @return bool
	 */
	public function cacheDelete() {
		if(empty($this->_cache_info['cache_type'])) {
			return false;
		}
		
		if($this->_cache_info['cache']) {
			$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
			return \aquagear\cache\Cache::getCacheInstance($this->_cache_info['cache_type'])->deleteValue($uri . "/" . md5(\aquagear\application\ApplicationData::getURI(true)), $this->_cache_info['ttl']);
		}
		
		return false;
	}
}
?>
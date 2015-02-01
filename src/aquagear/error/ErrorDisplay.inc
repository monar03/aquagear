<?php
namespace aquagear\error;

/**
 * 
 * エラー画面の出力用クラス
 * 
 * @package aquagear
 * @subpackage error
 * 
 * @author motonari
 *
 */
class ErrorDisplay extends \aquagear\Object {
	/**
	 * エラー出力
	 * 
	 * @param int $error_code エラーコード
	 * @param array $param 出力パラメータ
	 */
	public static function errorDisplay($error_code, array $param=array()) {
		try {
			$error_disp_conf = \aquagear\holder\ConfigureHolder::load(\aquagear\application\ApplicationData::getAquagearServerEnv('system_configure_root') . '/error_display.ini', true);
			\aquagear\log\Logger::getLogInstance()->debug('load configure ' . var_export($error_disp_conf, true));
			if($error_disp_conf->getValue($error_code) !== false) {
				$conf = $error_disp_conf->getValue($error_code);
				
				$contents = self::_getContents(\aquagear\application\ApplicationData::getAquagearServerEnv('template_root') . '/' . $conf['path'], $param);
				self::_render($contents);
			}
			else {
				\aquagear\utility\HttpUtility::errorRedirect();
			}
		}
		catch (\Exception $e) {
			\aquagear\utility\HttpUtility::errorRedirect();
		}
	}
	
	/**
	 * コンテンツの取得.
	 * 
	 * @param string $template_path テンプレートパス
	 * 
	 * @return string
	 */
	private static function _getContents($template_path, array $param=array()) {
		$uri = \aquagear\holder\HttpParameterHolder::getInstance()->getURI();
		$contents = "";

		extract($param);
		extract(\aquagear\holder\DataHolder::getInstance()->getAllData());
		
		\aquagear\log\Logger::getLogInstance()->debug('use template file path [ template_path = ' . $template_path . ' ]');

		ob_start();
		ob_implicit_flush(0);
		include($template_path);
		$contents = ob_get_clean();
		
		if(empty($contents)) {
			throw new \aquagear\exception\CommandException('empty command view contents.');
		}
		
		return $contents;
	}
	
	/**
	 * レンダリングメソッド.
	 * 
	 * @return void
	 */
	private static function _render($contents) {
		echo $contents;
		exit;
	}
}
?>